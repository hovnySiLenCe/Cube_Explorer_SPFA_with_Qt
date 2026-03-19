#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QColor>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector2D>
#include <QPoint>
#include <GL/glu.h>

// 单个手指结构
struct GripperFinger {
    QVector3D basePos;      // 基座位置
    float length;           // 手指长度
    float width;            // 手指宽度
    float thickness;        // 手指厚度
    float openOffset;       // 开合偏移量
};

// 二指夹爪结构
struct DualFingerGripper {
    GripperFinger finger1;  // 手指 1
    GripperFinger finger2;  // 手指 2
    QVector3D centerPos;    // 夹爪中心位置
    QVector3D approachDir;  // 夹持方向（指向魔方中心）
    QVector3D openDir;      // 开合方向（两手指分离方向）
    float openAmount;       // 开合程度 0-1（0=闭合，1=完全打开）
    bool isActive;          // 是否激活
    float rotationAngle;    // 夹爪整体旋转角度（用于拧动魔方面）
};

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget* parent = nullptr);

    // 接收 54 个面颜色的函数
    void setFaceColors(const QVector<QColor>& colors);

    // 夹爪激活控制
    void setGripperFActive(bool active);
    void setGripperRActive(bool active);
    
    // 开合控制
    void setGripperFOpenAmount(float amount);  // 0-1
    void setGripperROpenAmount(float amount);
    void closeGripperF();
    void closeGripperR();
    void openGripperF();
    void openGripperR();
    
    // 旋转控制（带动魔方面转动）
    void rotateGripperF(float angle);
    void rotateGripperR(float angle);
    
    // 获取夹爪状态
    bool isGripperFClosed() const;
    bool isGripperRClosed() const;

protected:
    void initializeGL() override;
    void initGrippers();
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintGL() override;

private:
    // 存储 54 个面的颜色
    QVector<QColor> m_faceColors;

    // 视角控制（可选）
    float m_rotationAngleX = 0.0f;
    float m_rotationAngleY = 0.0f;
    QPoint m_lastMousePos;

    // 两个二指夹爪
    DualFingerGripper m_gripperF;  // F 面夹爪（Z 轴正方向）
    DualFingerGripper m_gripperR;  // R 面夹爪（X 轴正方向）

    // 夹爪控制
    int m_activeGripper;     // 当前激活的夹爪 0=无，1=F 面，2=R 面

    void transformFaceColors(int type);
    void drawGripper(const DualFingerGripper& gripper);
    void drawGripperFinger(const GripperFinger& finger,
        const QVector3D& openDir,
        float openAmount,
        bool isFirst);
    void drawGripperBase(const QVector3D& center, const QVector3D& approachDir);


};