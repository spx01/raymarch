#ifndef RAYMARCH_APP_GUI_HPP
#define RAYMARCH_APP_GUI_HPP

struct GLFWwindow;

class AppGUI {
public:
    AppGUI();
    ~AppGUI();
    void init(GLFWwindow *window);
    void draw() const;
    static void render();
};


#endif//RAYMARCH_APP_GUI_HPP
