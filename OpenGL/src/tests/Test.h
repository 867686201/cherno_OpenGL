#pragma once

#include <vector>
#include <functional>
#include <iostream>

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

    class TestMenu : public Test
    {
    public:
        TestMenu(Test*& currentTestPointer);
        ~TestMenu() {}

        void OnImGuiRender() override;

        template<typename T>
        void RegisterTest(const std::string& name) 
        { 
            m_Tests.emplace_back(std::make_pair(name, []() {return new T(); }));
        }  

    private:

        std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
        Test*& m_CurrentTest;
    };
}