#include "MyOpenGLWidget.h"

int faceTrans[4][54] = {
    {11, 14, 17, 10, 13, 16, 9, 12, 15, 29, 32, 35, 28, 31, 34, 27, 30, 33, 20, 23, 26, 19, 22, 25, 18, 21, 24, 38, 41, 44, 37, 40, 43, 36, 39, 42, 2, 5, 8, 1, 4, 7, 0, 3, 6, 51, 48, 45, 52, 49, 46, 53, 50, 47},
    {0, 1, 2, 3, 4, 5, 9, 12, 15, 29, 10, 11, 28, 13, 14, 27, 16, 17, 20, 23, 26, 19, 22, 25, 18, 21, 24, 38, 41, 44, 30, 31, 32, 33, 34, 35, 36, 37, 8, 39, 40, 7, 42, 43, 6, 45, 46, 47, 48, 49, 50, 51, 52, 53},
    {53, 52, 51, 50, 49, 48, 47, 46, 45, 11, 14, 17, 10, 13, 16, 9, 12, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42, 39, 36, 43, 40, 37, 44, 41, 38, 35, 34, 33, 32, 31, 30, 29, 28, 27},
    {0, 1, 51, 3, 4, 48, 6, 7, 45, 11, 14, 17, 10, 13, 16, 9, 12, 15, 18, 19, 2, 21, 22, 5, 24, 25, 8, 27, 28, 20, 30, 31, 23, 33, 34, 26, 36, 37, 38, 39, 40, 41, 42, 43, 44, 35, 46, 47, 32, 49, 50, 29, 52, 53}
};
// 为了方便，这里采用立即模式（兼容 OpenGL 2.1+），你也可以使用 VBO/VAO 提高性能
MyOpenGLWidget::MyOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    // 启用鼠标跟踪以实现旋转（可选）
    setMouseTracking(true);
}

void MyOpenGLWidget::setFaceColors(const QVector<QColor>& colors)
{
    if (colors.size() == 54) {
        m_faceColors = colors;
        update(); // 触发重绘
    }
}

void MyOpenGLWidget::setGripperFActive(bool active)
{
    m_gripperF.isActive = active;
    update();
}

void MyOpenGLWidget::setGripperRActive(bool active)
{
    m_gripperR.isActive = active;
    update();
}

void MyOpenGLWidget::setGripperFOpenAmount(float amount)
{
    m_gripperF.openAmount = qBound(0.0f, amount, 1.0f);
    update();
}

void MyOpenGLWidget::setGripperROpenAmount(float amount)
{
    m_gripperR.openAmount = qBound(0.0f, amount, 1.0f);
    update();
}

void MyOpenGLWidget::closeGripperF()
{
    m_gripperF.openAmount = 0.0f;
    update();
}

void MyOpenGLWidget::closeGripperR()
{
    m_gripperR.openAmount = 0.0f;
    update();
}

void MyOpenGLWidget::openGripperF()
{
    m_gripperF.openAmount = 1.0f;
    update();
}

void MyOpenGLWidget::openGripperR()
{
    m_gripperR.openAmount = 1.0f;
    update();
}

void MyOpenGLWidget::rotateGripperF(float angle)
{
    m_gripperF.rotationAngle += angle;
    if (m_gripperF.openAmount == 0.0f)
        transformFaceColors(( (angle < 0) ? 1 : 0 ) + ((m_gripperR.openAmount > 0.0f)? 0: 2) );
    update();
}

void MyOpenGLWidget::rotateGripperR(float angle)
{
    m_gripperR.rotationAngle += angle;
    if (m_gripperR.openAmount == 0.0f)
        transformFaceColors(((angle < 0) ? 1 : 0) + ((m_gripperF.openAmount > 0.0f) ? 4 : 6));
    update();
}

bool MyOpenGLWidget::isGripperFClosed() const
{
    return m_gripperF.openAmount < 0.1f;
}

bool MyOpenGLWidget::isGripperRClosed() const
{
    return m_gripperR.openAmount < 0.1f;
}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // 设置背景色为白色
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE); // 启用背面剔除，提高性能
    glFrontFace(GL_CCW);    // 默认逆时针为正面

    // 设置光照（如果需要简单的颜色显示，可以不启用光照，直接使用颜色）
    // 这里我们使用纯色，不启用光照

    initGrippers();
}

void MyOpenGLWidget::initGrippers()
{
    // 初始化 F 面夹爪（沿 Z 轴正方向夹持）
    m_gripperF.centerPos = QVector3D(0.0f, 0.0f, 0.75f);   // F 面前方
    m_gripperF.approachDir = QVector3D(0.0f, 0.0f, -1.0f); // 指向魔方中心
    m_gripperF.openDir = QVector3D(0.0f, 1.0f, 0.0f);      // 沿 Y 轴开合
    m_gripperF.openAmount = 0.0f;
    m_gripperF.isActive = true;
    m_gripperF.rotationAngle = 0.0f;

    // F 面夹爪的两个手指（上下分布）
    m_gripperF.finger1.basePos = QVector3D(0.0f, 1.1f, 0.0f);  // 左上手指
    m_gripperF.finger1.length = 1.0f;
    m_gripperF.finger1.width = 0.5f;
    m_gripperF.finger1.thickness = 0.2f;

    m_gripperF.finger2.basePos = QVector3D(0.0f, -1.1f, 0.0f); // 左下手指
    m_gripperF.finger2.length = 1.0f;
    m_gripperF.finger2.width = 0.5f;
    m_gripperF.finger2.thickness = 0.2f;

    // 初始化 R 面夹爪（沿 X 轴正方向夹持）
    m_gripperR.centerPos = QVector3D(0.75f, 0.0f, 0.0f);   // R 面右侧
    m_gripperR.approachDir = QVector3D(-1.0f, 0.0f, 0.0f); // 指向魔方中心
    m_gripperR.openDir = QVector3D(0.0f, 1.0f, 0.0f);      // 沿 Y 轴开合
    m_gripperR.openAmount = 0.0f;
    m_gripperR.isActive = true;
    m_gripperR.rotationAngle = 0.0f;

    // R 面夹爪的两个手指（上下分布）
    m_gripperR.finger1.basePos = QVector3D(0.0f, 1.1f,  0.0f);  // 上手指 td
    m_gripperR.finger1.length = 1.0f;
    m_gripperR.finger1.width = 0.5f;
    m_gripperR.finger1.thickness = 0.2f;

    m_gripperR.finger2.basePos = QVector3D(0.0f, -1.1f, 0.0f); // 下手指 td
    m_gripperR.finger2.length = 1.0f;
    m_gripperR.finger2.width = 0.5f;
    m_gripperR.finger2.thickness = 0.2f;

    m_activeGripper = 2;
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    // 设置视口
    glViewport(0, 0, w, h);

    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 透视投影：视角45度，宽高比，近平面0.1，远平面100.0
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(45.0f, aspect, 0.1f, 100.0f); // 注意：需要包含 glu 头文件或使用 QMatrix4x4

    // 回到模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
}
void MyOpenGLWidget::paintGL()
{
    // 步骤1：清除颜色缓冲和深度缓冲
    // glClear 用于清除帧缓冲区，这里清除颜色（之前绘制的颜色）和深度（用于遮挡关系）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 步骤2：重置模型视图矩阵为单位矩阵
    // 在开始设置新的视图变换前，需要将当前矩阵重置，避免叠加之前的变换
    glLoadIdentity();

    // 步骤3：设置相机位置和观察方向（视图变换）
    // gluLookAt 定义了一个虚拟相机：
    // - 第一个三元组 (3.0, 3.0, 5.0) 是相机位置，位于右前上方
    // - 第二个三元组 (0.0, 0.0, 0.0) 是观察目标点（魔方中心）
    // - 第三个三元组 (0.0, 1.0, 0.0) 是向上的方向，指定Y轴正方向为“上”
    // 这样设置后，相机会从右前上方看向魔方中心，保持画面正立
    gluLookAt(-2.0, 5.0, -2.0,   // 相机位置
        0.0, 0.0, 0.0,   // 观察点
        -1.0, 0.0, -1.0);  // 向上向量

    // 步骤4：应用鼠标控制的旋转（模型变换）
    // glRotatef 绕指定轴旋转当前模型视图矩阵，从而实现魔方的旋转效果
    // m_rotationAngleX 和 m_rotationAngleY 是在鼠标事件中累积的角度值
    glRotatef(m_rotationAngleX, 1.0f, 0.0f, 0.0f); // 绕X轴旋转（上下方向）
    glRotatef(m_rotationAngleY, 0.0f, 1.0f, 0.0f); // 绕Y轴旋转（左右方向）

    // 步骤5：如果没有颜色数据，则直接返回（不绘制）
    if (m_faceColors.isEmpty()) {
        return;
    }

    // 步骤6：定义魔方的基本尺寸参数
    // 整个魔方边长为 2
    float cubeSize = 2.0f;
    // 每个小块的边长 = 总边长 / 3
    float blockSize = cubeSize / 3.0f; // 每个小块的边长
    // 半个小块的边长，用于计算顶点偏移
    float halfSize = blockSize / 2.0f; // 半个边长

    // 步骤7：定义六个面的方向（法线）及其在颜色列表中的起始索引
    // 面顺序：上(U)、右(R)、前(F)、下(D)、左(L)、后(B)
    // 注意：这里的顺序与常见的魔方表示法可能略有不同，但保证了每个面9个颜色连续
    // 每个面的法线指向外部，用于后续确定顶点偏移方向
    const struct { QVector3D dir; int startIdx; } faceInfo[6] = {
        { QVector3D(0, 1, 0),  0 }, // 上 U：法线指向Y轴正方向，颜色索引从0开始
        { QVector3D(1, 0, 0), 9 },  // 右 R：法线指向X轴正方向，颜色索引从9开始
        { QVector3D(0, 0, 1), 18 }, // 前 F：法线指向Z轴正方向，颜色索引从18开始
        { QVector3D(0,-1, 0),  27 }, // 下 D：法线指向Y轴负方向，颜色索引从27开始
        { QVector3D(-1, 0, 0), 36 }, // 左 L：法线指向X轴负方向，颜色索引从36开始
        { QVector3D(0, 0,-1), 45 } // 后 B：法线指向Z轴负方向，颜色索引从45开始
    };

    // 步骤8：遍历六个面
    for (int f = 0; f < 6; ++f) {
        QVector3D normal = faceInfo[f].dir;      // 当前面的法线方向
        int baseIdx = faceInfo[f].startIdx;      // 当前面在颜色列表中的起始索引

        // 步骤9：遍历当前面的 3x3 网格（行和列）
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                // 计算当前小面在颜色列表中的实际索引
                int idx = baseIdx + row * 3 + col;

                // 步骤10：计算小面中心的坐标
                // 首先计算在当前面的局部坐标系中，该小面相对于面中心的偏移
                // u：水平方向（从左到右），取值范围 -blockSize, 0, +blockSize
                // v：垂直方向（从上到下），取值范围 +blockSize, 0, -blockSize
                float u = (col - 1) * blockSize; // 列偏移
                float v = (row - 1) * blockSize; // 行偏移（注意1-row使第一行为正）

                float x, y, z; // 最终的世界坐标

                // 根据面的法线方向，将局部坐标(u, v)映射到世界坐标(x, y, z)
                if (normal == QVector3D(0, 1, 0)) { // 上
                    x = u;
                    y = halfSize * 3; // 上表面中心的Y坐标 = +1.5 * halfSize? 实际 halfSize*3 = blockSize*1.5 = (2/3)*1.5 = 1.0，即Y=1.0
                    z = v;
                }
                else if (normal == QVector3D(0, -1, 0)) { // 下
                    x = u;
                    y = -halfSize * 3; // Y = -1.0
                    z = -v;
                }
                else if (normal == QVector3D(0, 0, 1)) { // 前
                    x = u;
                    y = -v;
                    z = halfSize * 3; // Z = +1.0
                }
                else if (normal == QVector3D(0, 0, -1)) { // 后
                    x = -u;
                    y = -v;
                    z = -halfSize * 3; // Z = -1.0
                }
                else if (normal == QVector3D(-1, 0, 0)) { // 左
                    x = -halfSize * 3; // X = -1.0
                    y = -v;
                    z = u; // 注意：对于左右面，用u作为Z方向偏移，保证局部坐标一致
                }
                else { // 右 (normal == (1,0,0))
                    x = halfSize * 3; // X = +1.0
                    y = -v;
                    z = -u;
                }

                // 步骤11：获取该小面应显示的颜色，如果索引无效则默认灰色
                QColor color = m_faceColors.value(idx, Qt::gray);
                // 设置OpenGL当前颜色（用于后续顶点着色）
                glColor3f(color.redF(), color.greenF(), color.blueF());

                // 步骤12：开始绘制一个四边形（由两个三角形组成）
                glBegin(GL_TRIANGLES);

                // 步骤13：根据面的法线确定两个局部基向量：right（向右）和 up（向上）
                // 这些基向量用于构造小面的四个顶点偏移
                QVector3D right, up;
                if (normal == QVector3D(0, 1, 0) || normal == QVector3D(0, -1, 0)) {
                    // 对于上下表面，right沿X轴，up沿Z轴
                    right = QVector3D(1, 0, 0);
                    up = QVector3D(0, 0, 1);
                }
                else if (normal == QVector3D(0, 0, 1) || normal == QVector3D(0, 0, -1)) {
                    // 对于前后表面，right沿X轴，up沿Y轴
                    right = QVector3D(1, 0, 0);
                    up = QVector3D(0, 1, 0);
                }
                else { // 左右表面，right沿Z轴，up沿Y轴
                    right = QVector3D(0, 0, 1);
                    up = QVector3D(0, 1, 0);
                }

                // 步骤14：计算小面的四个顶点坐标（相对于小面中心）
                // p0: 左下角，p1: 右下角，p2: 右上角，p3: 左上角
                // 顺序为逆时针，保证面法线向外（符合OpenGL默认正面为逆时针）
                QVector3D p0 = QVector3D(x, y, z) + (-right * halfSize) + (-up * halfSize);
                QVector3D p1 = QVector3D(x, y, z) + (right * halfSize) + (-up * halfSize);
                QVector3D p2 = QVector3D(x, y, z) + (right * halfSize) + (up * halfSize);
                QVector3D p3 = QVector3D(x, y, z) + (-right * halfSize) + (up * halfSize);

                // 三角形1: p0-p1-p2
                glVertex3f(p0.x(), p0.y(), p0.z());
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p2.x(), p2.y(), p2.z());

                // 三角形2: p0-p2-p3
                glVertex3f(p0.x(), p0.y(), p0.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p3.x(), p3.y(), p3.z());

                glEnd(); // 结束当前小面的绘制

                //glDepthFunc(GL_LEQUAL);    // 允许深度值相等时通过（边框与填充面深度相同）
                //glDepthMask(GL_FALSE);     // 禁止写入深度缓冲区，避免边框影响后续深度测试
                glLineWidth(2.0f);

                glColor3f(0.0f, 0.0f, 0.0f); // 黑色边框
                // 用 GL_LINE_LOOP 绘制正方形边框
                glBegin(GL_LINE_LOOP);
                glVertex3f(p0.x(), p0.y(), p0.z());
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p3.x(), p3.y(), p3.z());
                glEnd();

                // 恢复深度测试设置，以便后续绘制正常
                //glDepthFunc(GL_LESS);
               // glDepthMask(GL_TRUE);
            }
        }
    }
    // 绘制两个夹爪
    drawGripper(m_gripperF);
    drawGripper(m_gripperR);
}

void MyOpenGLWidget::transformFaceColors(int type)
{
    QVector<QColor> tmp(m_faceColors);
    // 根据 type 应用颜色变换（使用 faceTrans 表）
    // type: 0=F 顺时针 带转，2=F 顺时针拧动，4=R 顺时针 带转，6=R 顺时针 拧动
    if (type&1) { // 逆时针拧动
        type >>= 1;
        for (int i = 0; i < 54; ++i) {
            m_faceColors[i] = tmp[faceTrans[type][i]];
        }
    }
    else { // 顺时针拧动
        type >>= 1;
        for (int i = 0; i < 54; ++i) {
            m_faceColors[faceTrans[type][i]] = tmp[i];
        }
    }
}

// 可选：添加鼠标旋转交互
void MyOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastMousePos = event->pos();
    }
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        int dx = event->x() - m_lastMousePos.x();
        int dy = event->y() - m_lastMousePos.y();
        m_rotationAngleY += dx * 0.5f;
        m_rotationAngleX += dy * 0.5f;
        m_lastMousePos = event->pos();
        update();
    }
}

void MyOpenGLWidget::drawGripper(const DualFingerGripper& gripper)
{
    if (!gripper.isActive) {
        return;
    }

    glPushMatrix();

    // 应用夹爪整体旋转（绕夹持方向轴旋转）

    glTranslatef(gripper.centerPos.x(), gripper.centerPos.y(), gripper.centerPos.z());

    if (gripper.approachDir == QVector3D(-1.0f, 0.0f, 0.0f))
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    glRotatef(gripper.rotationAngle, 0.0f, 0.0f, - 1.0f);

    //glTranslatef(-gripper.centerPos.x(), -gripper.centerPos.y(), -gripper.centerPos.z());

    // 绘制两个手指
    drawGripperFinger(gripper.finger1, gripper.openDir, gripper.openAmount, true);
    drawGripperFinger(gripper.finger2, gripper.openDir, gripper.openAmount, false);

    glPopMatrix();
}

void MyOpenGLWidget::drawGripperFinger(const GripperFinger& finger, 
                                        const QVector3D& openDir, 
                                        float openAmount, 
                                        bool isFirst)
{
    glColor3f(0.6f, 0.6f, 0.7f);  // 银灰色手指

    // 计算开合偏移
    float offset = (isFirst ? 1.0f : -1.0f) * openAmount * 0.5f;
    QVector3D offsetVec = openDir * offset;

    glPushMatrix();
    glTranslatef(finger.basePos.x() + offsetVec.x(), 
                 finger.basePos.y() + offsetVec.y(), 
                 finger.basePos.z() + offsetVec.z());

    // 绘制长方体手指
    float w = finger.width/2;
    float h = finger.thickness/2;
    float l = finger.length;

    glBegin(GL_QUADS);
    // 绘制手指
    // z轴负方向
    glVertex3f(-w, -h, 0); glVertex3f(w, -h, 0);
    glVertex3f(w, h, 0); glVertex3f(-w, h, 0);
    // z轴正方向
    glVertex3f(-w, -offsetVec.y(), l); glVertex3f(w, -offsetVec.y(), l);
    glVertex3f(w, h, l); glVertex3f(-w, h, l);
    // y轴正方向
    glVertex3f(-w, h, 0); glVertex3f(w, h, 0);
    glVertex3f(w, h, l); glVertex3f(-w, h, l);
    // y轴负方向
    glVertex3f(-w, -h, 0); glVertex3f(w, -h, 0);
    glVertex3f(w, -h, l); glVertex3f(-w, -h, l);
    // x轴负方向
    glVertex3f(-w, -h, 0); glVertex3f(-w, h, 0);
    glVertex3f(-w, h, l); glVertex3f(-w, -h, l);
    // x轴正方向
    glVertex3f(w, -h, 0); glVertex3f(w, h, 0);
    glVertex3f(w, h, l); glVertex3f(w, -h, l);

    // 绘制手指底
    // z轴负方向
    glVertex3f(-w, 0, l-h); glVertex3f(w, 0, l-h);
    glVertex3f(w, -offsetVec.y(), l-h); glVertex3f(-w, -offsetVec.y(), l-h);

    // x轴负方向
    glVertex3f(-w, 0, l - h); glVertex3f(-w, 0, l);
    glVertex3f(-w, -offsetVec.y(), l); glVertex3f(-w, -offsetVec.y(), l - h);

    // z轴负方向
    glVertex3f(w, 0, l - h); glVertex3f(w, 0, l);
    glVertex3f(w, -offsetVec.y(), l); glVertex3f(w, -offsetVec.y(), l - h);
   
    glEnd();

    glPopMatrix();
}