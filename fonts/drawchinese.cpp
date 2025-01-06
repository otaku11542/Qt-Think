#include "DrawChinese.h"
#include <QDir>
DrawChinese::DrawChinese(QObject *parent) : QObject(parent)
{
}
// 获取可执行文件的目录
QString executableDir = QDir::currentPath();
void DrawChinese::paint_chinese(cv::Mat& image, cv::Point start, unsigned long offset, cv::Scalar color)
{
    cv::Point p = start;
    FILE *HZK;
    char buff[72];//72个字节，用来存放汉字的
    if((HZK=fopen((executableDir+"/fonts/HZKf2424.hz").toStdString().c_str(),"rb"))==NULL){
        printf("Can't open HZKf2424.hz,Please check the path!");
        cout<<(executableDir+"/fonts/HZKf2424.hz").toStdString()<<endl;
        exit(0);//退出
    }
    fseek(HZK, offset, SEEK_SET);/*将文件指针移动到偏移量的位置*/
    fread(buff, 72, 1, HZK);/*从偏移量的位置读取72个字节，每个汉字占72个字节*/
    bool mat[24][24];//定义一个新的矩阵存放转置后的文字字膜
    int i,j,k;
    for (i = 0; i<24; i++)                 /*24x24点阵汉字，一共有24行*/
    {
        for (j = 0; j<3; j++)                /*横向有3个字节，循环判断每个字节的*/
            for (k = 0; k<8; k++)              /*每个字节有8位，循环判断每位是否为1*/
                if (buff[i * 3 + j] & (0x80 >> k))    /*测试当前位是否为1*/
                {
                    mat[j * 8 + k][i] = true;          /*为1的存入新的字膜中*/
                }
                else {
                    mat[j * 8 + k][i] = false;
                }
    }
    int scale = 5; // 放大6倍数
    for (i = 0; i < 24; i++)
    {
        p.x = start.x;
        for (j = 0; j < 24; j++)
        {
            if (mat[i][j])
            {
                // 绘制10x10的矩形来代替单个像素点
                rectangle(image, cv::Point(p.x, p.y), cv::Point(p.x + scale, p.y + scale), color, -1);
            }
            p.x += scale; // 右移一个像素点的宽度
        }
        p.y += scale; // 下移一个像素点的高度
    }
    fclose(HZK); // 关闭文件
}
void DrawChinese::paint_ascii(cv::Mat &image, cv::Point start, unsigned long offset, cv::Scalar color)
{
    cv::Point p = start;
    //存放ascii字膜
    char buff[16];
    //打开ascii字库文件
    FILE *ASCII;
    if ((ASCII = fopen((executableDir+"/fonts/Asci0816.zf").toStdString().c_str(), "rb")) == NULL){
        printf("Can't open ascii.zf,Please check the path!");
        cout<<(executableDir+"/fonts/Asci0816.zf").toStdString()<<endl;
        //getch();
        exit(0);
    }
    fseek(ASCII, offset, SEEK_SET);
    fread(buff, 16, 1, ASCII);
    int i, j;
    cv::Point p1 = p;
    for (i = 0; i<16; i++)                  //十六个char
    {
        p.x = start.x;
        for (j = 0; j < 8; j++)              //一个char八个bit
        {
            p1 = p;
            if (buff[i] & (0x80 >> j))    /*测试当前位是否为1*/
            {
                circle(image, p1, 0, color, -1);
                p1.x++;
                circle(image, p1, 0, color, -1);
                p1.y++;
                circle(image, p1, 0, color, -1);
                p1.x--;
                circle(image, p1, 0, color, -1);
            }
            p.x+=2;            //原来的一个像素点变为四个像素点，所以x和y都应该+2
        }
        p.y+=2;
    }
    fclose(ASCII); // 关闭文件
}

void DrawChinese::put_text_to_image(cv::Mat &image, QString text, cv::Scalar color)
{
    unsigned char *hexcode = qstringToHex(text);
    unsigned char qh, wh;      //定义区号，位号
    unsigned long offset;           //偏移量
    int x = 100, y = 160; //x,y:在图片上绘制文字的起始坐标
    int spaceWidth = 100; // 增加字符间距
    for(int m = 0; m < strlen((char *)hexcode);){
        if(hexcode[m] == 0x23){
            break;//读到#号时结束
        } else if(hexcode[m] > 0xaf){
            qh = hexcode[m] - 0xaf; //使用的字库里是以汉字啊开头，而不是以汉字符号开头
            wh = hexcode[m + 1] - 0xa0; //计算位码
            offset = (94 * (qh - 1) + (wh - 1)) * 72L;
            paint_chinese(image, cv::Point(x, y), offset, color);
            m += 2; //一个汉字的机内码占两个字节
            x += 24 + spaceWidth; // 一个汉字为24*24个像素点，加上100像素的水平间隔
            if (x > image.cols - 24) { // 如果超出图片宽度，则换行
                x = 100;
                y += 24 + spaceWidth; // 换行并增加100像素的垂直间隔
            }
        } else {
            wh = hexcode[m];
            offset = wh * 16L; //计算英文字符的偏移量
            paint_ascii(image, cv::Point(x, y), offset, color);
            m++; //英文字符在文件里表示只占一个字节
            x += 16 + spaceWidth; // 一个ASCII字符为16*8个像素点，加上100像素的水平间隔
            if (x > image.cols - 16) { // 如果超出图片宽度，则换行
                x = 100;
                y += 16 + spaceWidth; // 换行并增加100像素的垂直间隔
            }
        }
    }
    delete [] hexcode;
}

unsigned char *DrawChinese::qstringToHex(QString str)
{
    str += "#";
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    QByteArray bytegbkHex = gbk->fromUnicode(str).toHex();
    unsigned char *hexcode = new unsigned char[bytegbkHex.length()/2+1];
    for (int i = 0; i < bytegbkHex.length(); i += 2) {
        hexcode[i/2] = bytegbkHex.mid(i, 2).toInt(nullptr, 16);
    }
    return hexcode;
}
