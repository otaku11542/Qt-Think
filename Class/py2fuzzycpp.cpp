// 运行之前要把conda的python环境配置一下
// python>=3.9
// pip install fuzzywuzzy
// pip install update
//然后把下面地址改一下
#include "py2fuzzycpp.h"
Py2FuzzyCpp::Py2FuzzyCpp() {
    SetEnvironmentVariable(TEXT("PYTHONHOME"), TEXT("E:\\anaconda\\envs\\paddle_env"));
    SetEnvironmentVariable(TEXT("PYTHONPATH"), TEXT("E:\\anaconda\\envs\\paddle_env"));
    try {
        // 设置 C++ 标准输出流使用 UTF-8 编码
        std::locale::global(std::locale(std::locale(""), new std::codecvt_utf8<wchar_t>));
        // 初始化 Python 解释器
        Py_Initialize();
        if (!Py_IsInitialized()) {
            std::cerr << __LINE__ << " Python 初始化失败" << std::endl;
            return;
        }

        // 添加当前路径到 sys.path
        std::string path = QDir::currentPath().toStdString();
        std::string command = "import sys\nsys.path.append('" + path + "')";
        if (PyRun_SimpleString(command.c_str()) != 0) {
            std::cerr << __LINE__ << " 添加路径到 sys.path 失败" << std::endl;
            Py_Finalize();
            return;
        }

        isOpen = true;
    }
    catch (const std::exception& e) {
        std::cerr << __LINE__ << " 异常: " << e.what() << std::endl;
        Py_Finalize();
    }
}

Py2FuzzyCpp::~Py2FuzzyCpp()
{
    if (Py_IsInitialized()) {
        Py_Finalize();  // 关闭 Python 解释器
    }
}
//UTF8变成宽字符串
std::wstring utf8_to_wstring(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

OCRCharacters Py2FuzzyCpp::OCRFindText(const QString text, const QString tag)
{
    clock_t start =clock();
    OCRCharacters ocrct{};
    PyGILState_STATE gstate = PyGILState_Ensure(); // 确保获取 GIL

    try {
        // 导入 Python 模块
        PyObject* module = PyImport_ImportModule("Testone");

        if (!module) {
            PyErr_Print();
            std::cerr << __LINE__ << ": 无法导入模块 Testone.py" << std::endl;
            isOpen = false;
            return ocrct;
        }

        // 获取函数对象
        PyObject* hello_func = PyObject_GetAttrString(module, "fuzzy_fuzzy");
        if (!hello_func || !PyCallable_Check(hello_func)) {
            if (PyErr_Occurred()) PyErr_Print();
            std::cerr << __LINE__ << ": 函数 fuzzy_fuzzy() 未找到或不可调用" <<"使用time:"<<(double)((clock()-start)/100.00)<< std::endl;
            Py_XDECREF(hello_func);
            Py_DECREF(module);
            isOpen = false;
            return ocrct;
        }

        // 创建参数元组
        PyObject* pArgs = PyTuple_New(2);
        PyObject* pValue1 = PyUnicode_FromString(tag.toStdString().c_str());
        PyObject* pValue2 = PyUnicode_FromString(text.toStdString().c_str());
        if (!pValue1 || !pValue2) {
            std::cerr << __LINE__ << ": 创建参数失败" <<"使用time:"<<(double)((clock()-start)/100.00)<< std::endl;
            Py_XDECREF(pValue1);
            Py_XDECREF(pValue2);
            Py_DECREF(pArgs);
            Py_DECREF(hello_func);
            Py_DECREF(module);
            isOpen = false;
            return ocrct;
        }
        PyTuple_SetItem(pArgs, 0, pValue1);  // pValue1 被引用
        PyTuple_SetItem(pArgs, 1, pValue2);  // pValue2 被引用

        // 调用函数
        PyObject* pValue = PyObject_CallObject(hello_func, pArgs);

        Py_DECREF(pArgs);
        Py_DECREF(hello_func);
        Py_DECREF(module);

        if (pValue != nullptr) {
            if (PyTuple_Check(pValue)) {
                // 解析返回的元组
                if (PyTuple_Size(pValue) >= 2) {
                    PyObject* pReturnInt = PyTuple_GetItem(pValue, 0);
                    PyObject* pReturnString = PyTuple_GetItem(pValue, 1);

                    if (PyLong_Check(pReturnInt)) {
                        long returnInt = PyLong_AsLong(pReturnInt);
                        ocrct.similarity = static_cast<double>(returnInt) / 100.0;
                    } else {
                        std::cerr << __LINE__ << ": 返回的第一个值不是整数" << std::endl;
                    }

                    if (PyUnicode_Check(pReturnString)) {
                        std::string utf8_str = PyUnicode_AsUTF8(pReturnString);
                        ocrct.test = QString::fromStdString(utf8_str);
                    } else {
                        std::cerr << __LINE__ << ": 返回的第二个值不是字符串" << std::endl;
                    }
                } else {
                    std::cerr << __LINE__ << ": 返回的元组大小不足" << std::endl;
                }
            } else {
                std::cerr << __LINE__ << ": 函数未返回元组" << std::endl;
            }

            Py_DECREF(pValue);
        }
        else {
            PyErr_Print();
            std::cerr << "调用 Python 函数失败" <<"使用time:"<<(double)((clock()-start)/100.00)<< std::endl;
            isOpen = false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << __LINE__ << " 异常: " << e.what() <<"使用time:"<<(double)((clock()-start)/100.00)<< std::endl;
    }

    PyGILState_Release(gstate); // 释放 GIL
    return ocrct;
}

OCRCharacter Py2FuzzyCpp::OCR2FindAll(const QString tag, const QStringList data)
{
    clock_t start = clock();
    OCRCharacter ocrct;
    {
        ocrct.id = {};
        ocrct.similarity = 0.0;
        ocrct.test = "【"+tag+"】";
    }

    PyGILState_STATE gstate = PyGILState_Ensure(); // 确保获取 GIL

    try {
        // 导入 Python 模块
        PyObject* module = PyImport_ImportModule("Testone");

        if (!module) {
            PyErr_Print();
            std::cerr << __LINE__ << ": 无法导入模块 Testone.py" <<"  "
                      <<"使用time:"<<(double)((clock()-start)/1000.00)<< std::endl;
            isOpen = false;
            PyGILState_Release(gstate);
            return ocrct;
        }

        // 获取函数对象
        PyObject* pFunc = PyObject_GetAttrString(module, "differentials");

        if (!pFunc || !PyCallable_Check(pFunc)) {
            if (PyErr_Occurred()) PyErr_Print();
            std::cerr << __LINE__ << ": 函数 differentials() 未找到或不可调用"<<"  "
                      <<"使用time:"<<(double)((clock()-start)/1000.00) << std::endl;
            Py_XDECREF(pFunc);
            Py_DECREF(module);
            isOpen = false;
            PyGILState_Release(gstate);
            return ocrct;
        }

        // 创建参数元组
        PyObject* pArgs = PyTuple_New(2);
        PyObject* pStandard = PyUnicode_FromString(tag.toStdString().c_str());
        PyObject* pTestData = PyList_New(data.size());
        for (int i = 0; i < data.size(); ++i) {
            std::wstring wide_str = utf8_to_wstring(data[i].toStdString());
            PyObject* pItem = PyUnicode_FromWideChar(wide_str.c_str(), wide_str.length());
            if(!pItem){
                std::cerr<<"错误："<<__FUNCTION__<<":"<<__LINE__<<":该元素无法放入："
                          <<data[i].toStdString()<<std::endl;
                pItem = PyUnicode_FromString("NULL");
            }
            PyList_SetItem(pTestData, i, pItem); // PyList_SetItem steals reference
        }
        PyTuple_SetItem(pArgs, 0, pStandard);  // pStandard 被引用
        PyTuple_SetItem(pArgs, 1, pTestData);  // pTestData 被引用

        // 调用函数
        PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

        if(((clock()-start)/1000.000)>1){
    std::cout<<"tag:"<<tag.toStdString()<<std::endl;
            std::cout<<"data:"<<std::endl;
    for(auto str : data){
        std::cout<<str.toStdString()<<std::endl;
    }
        }
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        Py_DECREF(module);

        if (pResult == nullptr) {
            PyErr_Print();
            std::cerr << "调用 Python 函数失败" <<"  "
                      <<"使用time:"<<(double)((clock()-start)/1000.00)<< std::endl;
            isOpen = false;
            PyGILState_Release(gstate);
            return ocrct;
        }

        // 解析返回的元组
        if (PyTuple_Check(pResult)) {
            if (PyTuple_Size(pResult) >= 3) {
                // 获取匹配索引列表
                PyObject* pIndices = PyTuple_GetItem(pResult, 0);

                if (PyList_Check(pIndices)) {
                    for (Py_ssize_t i = 0; i < PyList_Size(pIndices); ++i) {
                        PyObject* pItem = PyList_GetItem(pIndices, i);
                        if (PyLong_Check(pItem)) {
                            ocrct.id.push_back(static_cast<int>(PyLong_AsLong(pItem)));
                        }
                    }
                } else {
                    std::cerr << __LINE__ << ": 第一个返回值不是列表" << std::endl;
                }

                // 获取修改后的字符串
                PyObject* pModifiedString = PyTuple_GetItem(pResult, 1);

                if (PyUnicode_Check(pModifiedString)) {
                    std::string modifiedStr = PyUnicode_AsUTF8(pModifiedString);
                    ocrct.test = QString::fromStdString(modifiedStr);
                } else {
                    std::cerr << __LINE__ << ": 第二个返回值不是字符串" << std::endl;
                }

                // 获取相似度分数
                PyObject* pScore = PyTuple_GetItem(pResult, 2);

                if (PyLong_Check(pScore)) {
                    double score = PyLong_AsLong(pScore);
                    ocrct.similarity = score / 100.0;
                } else {
                    std::cerr << __LINE__ << ": 第三个返回值不是整数" << std::endl;
                }
            } else {
                std::cerr << __LINE__ << ": 返回的元组大小不足" << std::endl;
            }
        } else {
            std::cerr << __LINE__ << ": 函数未返回元组" << std::endl;
        }

        Py_DECREF(pResult);
    }
    catch (const std::exception& e) {
        std::cerr << __LINE__ << " 异常: " << e.what() <<"  "
                  <<"使用time:"<<(double)((clock()-start)/1000.00)<< std::endl;
    }

    PyGILState_Release(gstate); // 释放 GIL
    return ocrct;
}

QString Py2FuzzyCpp::extractString(const QString &input)
{
    // 去除字符串末尾的空格
    std::string trimmedInput = input.toStdString();
    trimmedInput.erase(trimmedInput.find_last_not_of(' ') + 1);
    std::string openBracket = "【";
    std::string closeBracket = "】";

    // 查找最后一个closeBracket的位置
    size_t lastCloseBracketPos = trimmedInput.rfind(closeBracket);
    if (lastCloseBracketPos == std::string::npos) {
        return "aaa";
    }

    // 检查最后一个closeBracket是否在字符串的结尾
    if (lastCloseBracketPos != trimmedInput.length() - closeBracket.length()) {
        return "aaa";
    }

    // 从最后一个closeBracket的位置向前查找第一个openBracket的位置
    size_t lastOpenBracketPos = trimmedInput.rfind(openBracket, lastCloseBracketPos);
    if (lastOpenBracketPos == std::string::npos) {
        return "aaa";
    }

    // 返回openBracket和closeBracket之间的字符串
    return QString::fromStdString(trimmedInput.substr(lastOpenBracketPos + openBracket.length(),
                                                      lastCloseBracketPos - lastOpenBracketPos - openBracket.length()));
}

QString Py2FuzzyCpp::replaceLastBrackets(const QString &input, const QString &replacement)
{
    std::string openBracket = "【";
    std::string closeBracket = "】";
    std::string trimmedInput = input.toStdString();
    trimmedInput.erase(trimmedInput.find_last_not_of(' ') + 1);
    size_t closeBracketPos = trimmedInput.rfind(closeBracket);
    if (closeBracketPos != std::string::npos) {
        size_t openBracketPos = trimmedInput.rfind(openBracket, closeBracketPos);
        if (openBracketPos != std::string::npos) {
            // 替换最后一个【】对为replacement
            return QString::fromStdString(trimmedInput.substr(0, openBracketPos) + replacement.toStdString() + trimmedInput.substr(closeBracketPos + closeBracket.length()));
        }
    }
    // 如果没有找到【】对，返回原始字符串
    return input;
}
