#pragma once

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
}