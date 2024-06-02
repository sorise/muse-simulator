#ifndef MUSE_SIMULATOR_TIMER_NODE_HPP
#define MUSE_SIMULATOR_TIMER_NODE_HPP

#include <memory>
#include <memory_resource>
#include <functional>

#include "../utils/toolkits.hpp"
#include "simulator/simulator_world_state.hpp"

namespace muse::simulator {
    /*
     * 时间节点
     * */
    class SIMULATOR_CPP_WIN_API timer_node {
    public:
        using CallBack = std::function<void()>;
    private:
        //啥时候添加上tree的
        uint64_t add_time_;
        //啥时候过期
        uint64_t expire_;
        //是否重复执行
        bool duplicate_;
        //间隔
        uint64_t interval_;
        //运行次数
        uint32_t run_times_;
        //是否需要停止
        std::atomic<bool> clear_state_{false};

        CallBack callBack; //回调函数

        uint64_t id_;

        std::string name_;
    public:
        timer_node(const uint64_t &_id,CallBack ca, bool duplicate, uint64_t interval,uint64_t expire);

        [[nodiscard]] const std::string& get_task_name() const;

        void set_task_name(const std::string& _name);

        [[nodiscard]] uint64_t get_id() const;

        void set_id(const uint64_t& _id);

        [[nodiscard]] uint64_t get_expire() const;

        [[nodiscard]] bool is_duplicate() const;

        [[nodiscard]] uint64_t get_internal() const;

        [[nodiscard]] uint64_t get_add_time() const;

        bool get_clear_state() const;

        void duplicate_run_setting();

        void trigger() const;

        void stop();
    };

    struct SIMULATOR_CPP_WIN_API timer_node_wrapper {
        friend  bool operator < (const timer_node_wrapper &me, const timer_node_wrapper &other);
    private:
        std::shared_ptr<timer_node> node;
    public:
        timer_node_wrapper() = delete;

        explicit timer_node_wrapper(std::shared_ptr<timer_node> _node);

        timer_node_wrapper(const timer_node_wrapper &other);

        timer_node_wrapper(timer_node_wrapper &&other) noexcept;

        timer_node_wrapper &operator=(const timer_node_wrapper &other);

        timer_node_wrapper &operator=(timer_node_wrapper &&other) noexcept;

        void stop();

        void trigger() const;

        void duplicate_run_setting();

        [[nodiscard]] bool is_duplicate() const;

        [[nodiscard]] uint64_t expire() const;

        [[nodiscard]] bool get_clear_state() const;

        [[nodiscard]] uint64_t get_id() const;

        [[nodiscard]] const std::string& get_task_name() const;
    };


}
#endif //MUSE_SIMULATOR_TIMER_NODE_HPP
