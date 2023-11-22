#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// GL/glew.h ��Ҫ�� GLFW ��ͷ�ļ�֮ǰ

int main(void)
{
    GLFWwindow* window;

    /* ��ʼ���� */
    if (!glfwInit())
        return -1;

    /* ���� window ������ OpenGL ������ */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /*  ��ָ�����ڵ� OpenGL ����������Ϊ��ǰ������
        ��ʹ�� OpenGL ����ͼ����Ⱦʱ�������ж�����ڣ�ÿ�����ڶ����Լ��� OpenGL ������
        �����ı�ʾ�� OpenGL �ĵ�ǰ״̬��������ǰ����Ⱦ���á�״̬���õ�
        ͨ������ glfwMakeContextCurrent ����������Ը��� GLFW ʹ��ָ�����ڵ���������Ϊ��ǰ�����ģ��Ա������ OpenGL �����ܹ��ڸô�������Ч��
    */
    glfwMakeContextCurrent(window);

    // �����ڳ�ʼ��������֮����� glewInit
    if (glewInit() != GLEW_OK)
        std::cout << "glewInitError" << std::endl;

    // ����һ����Ч�� OpenGL ������֮�󣬿�������汾
    // 4.6.14756 Compatibility Profile Context 20.40.52 27.20.14052.10
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float position[6] =
    {
         0.5f,  0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // ���û�гɹ� glewInit��������غ���ʱ���׳��쳣
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);  // �鿴�ĵ� docs.gl

    glEnableVertexAttribArray(0);
    /**
     * glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
     * @param index, ���Ե�����, ��λ��������0������������1������������2
     * @param size, �����Ե� count, ÿ�������ĳ�������ж��ٸ� count, ������ÿ�������λ�������� 2 ��
     * @param type, ��������
     * @param normalized, ֻ��������, ��һ���� [0, 1] �� [-1, 1] (�����Ƿ����з�������)
     * @param stride, ����������ֽڴ�С
     * @param pointer, ���� offset ���ֽڴ�С
     */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    /* ѭ��ֱ���رմ��� */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);   // ƬԪ���͡������������ʼ���������ƶ��ٸ�����


        /* ����ǰ�󻺳��� */
        glfwSwapBuffers(window);

        /* ������ѯ�¼� */
        glfwPollEvents();
    }

    // �ͷ��� GLFW ��ص���Դ����ȷ����ȷ�عرպ����� GLFW ���״̬
    glfwTerminate();
    return 0;
}