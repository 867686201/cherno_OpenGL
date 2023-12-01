#pragma once

#include <vector>
#include <functional>
#include <iostream>

namespace test
{
    class Test
    {
    public:
        Test() {}  								  // �ڹ��캯�������ö������ݵ�
        virtual ~Test() {}

        virtual void OnUpdate(float deltaTime) {} // �����Ǹ������֮���
        virtual void OnRender() {}                // loop �е� drawcall ������֮ǰ�����״̬
        virtual void OnImGuiRender() {}           // imgui ��ش���
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