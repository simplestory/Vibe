#ifndef _VIBE_H_
#define _VIBE_H_


#include<iostream>
#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>


class VIBE {
public:
    VIBE();
    ~VIBE();
    pybind11::array_t<unsigned char> Rgb2Gray(pybind11::array_t<unsigned char> &img_rgb);
    void Initial(pybind11::array_t<unsigned char> &img_gray);
    void Update(pybind11::array_t<unsigned char> &img_gray, pybind11::array_t<unsigned char> &segmap);
    pybind11::array_t<unsigned char> Vibe(pybind11::array_t<unsigned char> &img_rgb);

    void setsampleNums(int &sampleNums_) { sampleNums = sampleNums_; }
    const int &getsampleNums() const { return sampleNums; }
    void setminiMatch(int &miniMatch_) { miniMatch = miniMatch_; }
    const int &getminiMatch() const { return miniMatch; }
    void setradis(int &radis_) { radis = radis_; }
    const int &getradis() const { return radis; }
    void setrateNum(int &rateNum_) { rateNum = rateNum_; }
    const int &getrateNum() const { return rateNum; }

private:
    bool FIRST = true;  // 第一帧标志
    int BACK_GROUND = 0;  // 背景像素值
    int FONT_GROUND = 255;  // 前景像素值
    int FIELDRANGE = 3;  // 领域大小
    int FIELDNUM = 8;  // 目标领域像素点数
    int sampleNums = 20;  // 各像素模型数
    int miniMatch = 6;  // 最小背景匹配数
    int radis = 20;  // 背景模型最大偏差
    int rateNum = 16;  // 背景更新概率种子(1/rateNum)
    int cshift;  // 列偏移
    int rshift;  // 行偏移
    int FIELD[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};  // 邻域步长
    pybind11::array_t<unsigned char> bgmodel;  // 背景模型
};


PYBIND11_MODULE(VIBE, m) {
    m.doc() = "Background Creatation CPP Backend";
    
    pybind11::class_<VIBE>(m, "VIBE")
        .def(pybind11::init())
        .def("Rgb2Gray", &VIBE::Rgb2Gray, pybind11::return_value_policy::copy)
        .def("Initial", &VIBE::Initial)
        .def("Update", &VIBE::Update)
        .def("Vibe", &VIBE::Vibe, pybind11::return_value_policy::copy)

        .def_property("sampleNums", &VIBE::getsampleNums, &VIBE::setsampleNums)
        .def_property("miniMatch", &VIBE::getminiMatch, &VIBE::setminiMatch)
        .def_property("radis", &VIBE::getradis, &VIBE::setradis)
        .def_property("rateNum", &VIBE::getrateNum, &VIBE::setrateNum);
}

#endif /*_VIBE_H_*/