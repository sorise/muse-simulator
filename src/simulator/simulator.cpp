#include "simulator/simulator.hpp"
#include "simulator/machines/ipv4_address.hpp"

namespace muse::simulator{

    simulator::~simulator() = default;

    void simulator::initialize_simulator() {
        //注册主机
        if (MUSE_NETWORK_DISPATCHER::get_reference().get_host_count() <= 1){
            auto message = fmt::format("error 1, simulating the world requires more than two hosts. current host count: {}.", MUSE_NETWORK_DISPATCHER::get_reference().get_host_count());
            throw std::logic_error(message);
        }
        //检查是否有RPC绑定，没有就不会有网络交互
        if (MUSE_REGISTRY::get_reference().get_remote_functions() < 1){
            auto message = fmt::format("error 2, simulating the world requires more than two hosts. current host count: {}.", MUSE_NETWORK_DISPATCHER::get_reference().get_host_count());
            throw std::logic_error(message);
        }
        //调用初始化方法
        const auto hosts =  MUSE_NETWORK_DISPATCHER::get_ptr()->get_hosts_list();
        if (hosts.size() > MUSE_SIMULATOR_SETTING::HOST_USE_THREAD_COUNT){
            //主机数量太多就需要 需要并行算法优化
            std::list<std::shared_ptr<muse::pool::ExecutorToken<void>>> futures(hosts.size());

            for (const auto& host_ptr: hosts) {
                auto token = muse::pool::make_executor([](computer* cmp){
                    cmp->START_UP();//启动主机
                }, host_ptr.get());
                singleton_thread_pool::get_ptr()->commit_executor(token);
                futures.push_back(token);
            }
            //等待
            for (const auto& fu: futures) {
                fu->get();
            }
        }else{
            //单线程即可
            std::for_each(hosts.begin(), hosts.end(), [=](auto host_ptr){
                host_ptr->START_UP();
            });
        }
    }

    void simulator::simulator_operating_core() {
        //当前模拟世界时间
        uint64_t ms_tick = MUSE_SIMULATOR_WORLD_STATE::get_ptr()->get_tick();
        //mt::print("simulator word time:{} minute {} seconds {}ms \n",ms_tick/60000, ms_tick/1000,ms_tick);
        //网络中的所有主机
        const auto hosts =  MUSE_NETWORK_DISPATCHER::get_ptr()->get_hosts_list();

        if (hosts.size() > MUSE_SIMULATOR_SETTING::HOST_USE_THREAD_COUNT){
            //主机数量太多就需要 需要并行算法优化
            std::list<std::shared_ptr<muse::pool::ExecutorToken<void>>> futures(hosts.size());

            for (const auto& host_ptr: hosts) {
                auto token = muse::pool::make_executor([](computer* cmp, const uint64_t& _ms_tick)->void{
                    cmp->_next_tick(_ms_tick);//启动主机
                }, host_ptr.get(),std::ref(ms_tick));

                singleton_thread_pool::get_ptr()->commit_executor(token);
                futures.push_back(token);
            }

            //等待
            for (const auto& fu: futures) {
                fu->get();
            }
        }else{
            //先执行主机内部 网络传输和运行任务
            std::for_each(hosts.begin(), hosts.end(), [ms_tick](auto host_ptr){
                host_ptr->_next_tick(ms_tick);
            });
        }

        //执行网络事件
        simulator_net_event_queue::for_each([=](simulator_event& sev)->bool {
            if (sev.event_type_ == simulator_net_event_type::RPC_REQUEST_FINISH){
                //已经完成传输需要等待服务端进行处理
                auto _host = MUSE_NETWORK_DISPATCHER::get_reference().get_host(sev.message_->acceptor_id);
                //目标主机不存在，发出去就有问题
                if (_host == nullptr){
                    auto _sender = MUSE_NETWORK_DISPATCHER::get_reference().get_host(sev.message_->sender_ip);
                    //函数不存在
                    ResponseData *rp = MUSE_REGISTRY::get_ptr()->net_not_support_response();
                    //返回结果
                    sev.message_->response = rp;
                    //运行回调函数
                    sev.message_->request->trigger_callBack(sev.message_->response);
                    //_runtime 活动CPU运行时间
                    auto _runtime = MUSE_CPU_PROCESSING_MATRIX::get_ptr()->get_server(sev.message_->request->remote_process_name);
                    //运行
                    _sender->_run_on_core(ms_tick, _runtime);
                    /* 需要回收资源 */
                    delete_message_factory(sev.message_);
                    //回收消息
                    return true;
                }
                //获取目标主机
                auto space_cc = _host->_get_spare_core_count(ms_tick);
                if ( space_cc <= 0){
                    //没有主机没有空闲核心可以运行任务
                    return false;
                }
                //有主机空闲核心可以运行任务
                std::string rpc_name;
                //重置读取点
                sev.message_->request->get_serializer().reset();
                //解析 RPC 函数名
                sev.message_->request->get_serializer().output(rpc_name);
                //设置相应需要间隔多久发送
                sev.message_->cpu_processing_us = MUSE_CPU_PROCESSING_MATRIX::get_ptr()->get_server(rpc_name);
                //内容字节长度
                auto pure_data_count = sev.message_->request->get_serializer().byteCount();
                bool no_other_args = (pure_data_count ==  sev.message_->request->get_serializer().getReadPosition());
                //解析 RPC 函数名, 而且必须这样做，因为序列化器会移动读取点。不然直接调用会出问题。
                if (MUSE_REGISTRY::get_ptr()->check(rpc_name)){
                    /* no pro */
                    if (rpc_name.find(network_dispatcher::PREFIX) == 0){
                        ipv4_address ip(sev.message_->request->get_ip_address());
                        uint16_t client_port = sev.message_->request->get_port();

                        muse::serializer::BinarySerializer serializer_ip_port;
                        bool no_other_args = (pure_data_count == sev.message_->request->get_serializer().getReadPosition());
                        if (no_other_args){
                            serializer_ip_port.input(std::make_tuple(ip,client_port));
                        }else{
                            serializer_ip_port.inputArgs(ip, client_port);
                        }
                        //新的数据
                        auto new_pure_data_count = pure_data_count + serializer_ip_port.byteCount();
                        //重写分片
                        auto store = singleton_memory_pool::get_ptr()->allocate(new_pure_data_count);

                        std::shared_ptr<char[]> dt((char*)store, [=](char *ptr){
                            singleton_memory_pool::get_ptr()->deallocate(ptr, new_pure_data_count);
                        });
                        //复制数据
                        std::memcpy(store, sev.message_->request->get_serializer().getBinaryStream(), pure_data_count);
                        //复制数据
                        std::memcpy((char*)store + pure_data_count, serializer_ip_port.getBinaryStream(), serializer_ip_port.byteCount());

                        //有其他参数需要修改长度
                        if (!no_other_args){
                            auto pos = sev.message_->request->get_serializer().getReadPosition() + sizeof(muse::serializer::BinaryDataType);
                            auto char_pos = (char*)dt.get() + pos;

                            muse::serializer::BinarySerializer::Tuple_Element_Length
                                    tpl_size = *reinterpret_cast<uint16_t *>(char_pos); //元祖长度
                            //如果当前机器是大端序
                            // 小端序 ---> 大端序
                            if (this->sequence == muse::serializer::ByteSequence::BigEndian){
                                auto first = (char*)&tpl_size;
                                auto last = first + sizeof(muse::serializer::BinarySerializer::Tuple_Element_Length);
                                std::reverse(first, last);
                            }
                            tpl_size += 2; // 把数组长度  + 2
                            //如果是大端序，数据插回去
                            // 大端序 ---> 小端序
                            if (this->sequence == muse::serializer::ByteSequence::BigEndian){
                                auto first = (char*)&tpl_size;
                                auto last = first + sizeof(muse::serializer::BinarySerializer::Tuple_Element_Length);
                                std::reverse(first, last);
                            }
                            //覆盖原来的数组长度
                            std::memcpy(char_pos, (char*)&tpl_size, sizeof(muse::serializer::BinarySerializer::Tuple_Element_Length));
                        }

                        sev.message_->request->get_serializer().clear();
                        sev.message_->request->get_serializer().write(dt.get(),new_pure_data_count);
                        sev.message_->request->get_serializer().output(rpc_name);
                    }
                    //运行调用任务
                    auto _runtime = MUSE_CPU_PROCESSING_MATRIX::get_ptr()->get_server(rpc_name);
                    //_runtime 活动CPU运行时间
                    _host->_run_on_core(ms_tick, _runtime);
                    //在某个核心上运行
                    MUSE_REGISTRY::get_ptr()->runEnsured(_host, rpc_name, &(sev.message_->request->get_serializer()));
                    //调用 RPC 服务端
                    ResponseData *rp = MUSE_REGISTRY::get_ptr()->convert_result_to_response(&(sev.message_->request->get_serializer()));
                    //返回结果
                    sev.message_->response = rp;
                }
                else{
                    //函数不存在
                    ResponseData *rp = MUSE_REGISTRY::get_ptr()->function_not_exist_response();
                    //返回结果
                    sev.message_->response = rp;
                }
                //服务端已经触发
                sev.message_->rpc_server_is_trigger = true;
                _host->_add_task(sev.message_);
                return true;
            } else if (sev.event_type_ == simulator_net_event_type::RPC_RESPONSE_FINISH){
                //服务器已经处理完毕，并且已经发送完毕了
                auto _host = MUSE_NETWORK_DISPATCHER::get_reference().get_host(sev.message_->sender_ip);
                //获取目标主机
                auto space_cc = _host->_get_spare_core_count(ms_tick);
                if ( space_cc <= 0){
                    //没有主机没有空闲核心可以运行任务
                    return false;
                }
                //调用回调函数
                auto _runtime = MUSE_CPU_PROCESSING_MATRIX::get_ptr()->get_server(sev.message_->request->remote_process_name);
                //出问题了
                if (sev.message_->response == nullptr){
                    throw std::runtime_error("response is null in simulator_operating_core.simulator_net_event_queue::for_each.");
                }
                //运行回调函数
                sev.message_->request->trigger_callBack(sev.message_->response);
                //_runtime 活动CPU运行时间
                _host->_run_on_core(ms_tick, _runtime);
                //设置一下，好像没啥意义
                sev.message_->rpc_client_is_trigger = true;
                /* 需要回收资源 */
                delete_message_factory(sev.message_);
                //重置一下
                sev.message_ = nullptr;
                return true;
            }
            return true;
        });
    }

    bool simulator::stop_simulator_condition() {
        if (this->stop_function_) return this->stop_function_();
        return true;
    }

    void simulator::simulator_report() {
        if (this->simulator_report_){
            this->simulator_report_();
        }
    }

    void simulator::simulator_clean_up_resources() {
        //重新设置时间
        MUSE_SIMULATOR_WORLD_STATE::get_ptr()->reset_simulator_world();
    }

    void simulator::set_stop_condition(std::function<bool(void)>&& f){
        this->stop_function_ = std::move(f);
    }

    void simulator::set_report(std::function<void()>&& f){
        this->simulator_report_ = std::move(f);
    }

    void simulator::set_simulator_end_of_lifecycle(std::function<void()>&& f){
        this->simulator_clean_up_resources_ = std::move(f);
    }
}