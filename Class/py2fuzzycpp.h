#ifndef PY2FUZZYCPP_H
#define PY2FUZZYCPP_H
#include <iostream>
#include <Python.h>  // Python/C API头文件
#include <string>
#include <locale>
#include <codecvt>
#include <QDir>
#include <QString>
struct OCRCharacters{
    double similarity;
    QString test;
};
struct OCRCharacter{
    double similarity;
    QString test;
    std::vector<int> id;
};
class Py2FuzzyCpp
{
public:
    Py2FuzzyCpp();
    ~Py2FuzzyCpp();
    //字符串是否全有
    OCRCharacters OCRFindText(const QString text, const QString tag);
    //大字符串查询
    OCRCharacter OCR2FindAll(const QString tag, const QStringList data);
    //截取下半部分
    QString extractString(const QString& input);
    // 替换最后面的【】对为指定字符串
    QString replaceLastBrackets(const QString& input, const QString& replacement);
    bool isOpen = false;
private:
    PyObject* hello_func;
    PyObject* module;
};

#endif // PY2FUZZYCPP_H
