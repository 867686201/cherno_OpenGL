#pragma once

namespace test
{
    class Test
    {
    public:
        Test() {}  								  // 在构造函数中设置顶点数据等
        virtual ~Test() {}

        virtual void OnUpdate(float deltaTime) {} // 可能是更新相机之类的
        virtual void OnRender() {}                // loop 中的 drawcall 命令与之前的相关状态
        virtual void OnImGuiRender() {}           // imgui 相关代码
    };
}