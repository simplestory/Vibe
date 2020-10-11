
#include<iostream>
#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>
#include<vibe.hpp>


VIBE::VIBE() {};
VIBE::~VIBE() {};

pybind11::array_t<unsigned char> VIBE::Rgb2Gray(pybind11::array_t<unsigned char> &img_rgb) {
    if (img_rgb.ndim() != 3){
        throw std::runtime_error("RGB image has 3 channels.");
    }

    pybind11::array_t<unsigned char> img_gray = pybind11::array_t<unsigned char>(img_rgb.shape()[0] * img_rgb.shape()[1]);
    img_gray.resize({img_rgb.shape()[0], img_rgb.shape()[1]});

    auto rgb = img_rgb.unchecked<3>();
    auto gray = img_gray.mutable_unchecked<2>();

    for (int i=0; i < img_rgb.shape()[0]; i++){
        for (int j=0; j < img_rgb.shape()[1]; j++){
            auto R = rgb(i, j, 0);
            auto G = rgb(i, j, 1);
            auto B = rgb(i, j, 2);

            auto GRAY = (R*30 + G*50 + B*11 + 50) / 100;
            gray(i, j) = static_cast<unsigned char>(GRAY);
        }
    }

    return img_gray;
}


void VIBE::Initial(pybind11::array_t<unsigned char> &img_gray) {
    int height = img_gray.shape()[0];
    int width = img_gray.shape()[1];
    pybind11::array_t<unsigned char> background_model = pybind11::array_t<unsigned char>(height * width * sampleNums);
    background_model.resize({height, width, sampleNums});
    auto gray = img_gray.unchecked<2>();
    auto sampleval = background_model.mutable_unchecked<3>();

    for (int r=0; r < height; r++){
        for (int c=0; c < width; c++){
            if (c < FIELDRANGE/2 || c < width-FIELDRANGE-1 || r < FIELDRANGE/2 || r < height-FIELDRANGE-1){
                for (int idx=0; idx < sampleNums; idx++){
                    int index = rand()%FIELDNUM;
                    cshift = FIELD[index][0];
                    rshift = FIELD[index][1];
                    if (c+cshift < 0 || c+cshift >= width)
                        cshift *= -1;
                    if (r+rshift < 0 || r+rshift >= height)
                        rshift *= -1;
                    sampleval(r, c, idx) = gray(r+rshift, c+cshift);
                }
            }
            else{
                for (int idx=0; idx < sampleNums; idx++){
                    int index = rand()%FIELDNUM;
                    cshift = FIELD[index][0];
                    rshift = FIELD[index][1];
                    if (c+cshift < 0 || c+cshift >= width)
                        cshift *= -1;
                    if (r+rshift < 0 || r+rshift >= height)
                        rshift *= -1;
                    sampleval(r, c, idx) = gray(r+rshift, c+cshift);
                }
            }
        }
    }

    bgmodel = background_model;
}


void VIBE::Update(pybind11::array_t<unsigned char> &img_gray, pybind11::array_t<unsigned char> &segmap) {
    int height = img_gray.shape()[0];
    int width = img_gray.shape()[1];
    // pybind11::array_t<unsigned char> segmap = pybind11::array_t<unsigned char>(height * width);
    // segmap.resize({height, width});
    auto gray = img_gray.unchecked<2>();
    auto sampleval = bgmodel.mutable_unchecked<3>();
    auto segval = segmap.mutable_unchecked<2>();

    for (int r=0; r < height; r++){
        for (int c=0; c < width; c++){
            int count = 0;
            auto pixel = gray(r, c);

            for (int idx=0; idx < sampleNums; idx++){
                if (abs(int(pixel)-int(sampleval(r, c, idx))) < radis){
                    count ++;

                    if (count >= miniMatch){
                        if (rand()/double(RAND_MAX) <= 1/rateNum)
                            sampleval(r, c, idx) = pixel;

                        if (rand()/double(RAND_MAX) <= 1/rateNum){
                            if (c < FIELDRANGE/2 || c > width-FIELDRANGE/2-1 || r < FIELDRANGE/2 || r > height-FIELDRANGE/2-1){
                                int index = rand()%FIELDNUM;
                                cshift = FIELD[index][0];
                                rshift = FIELD[index][1];
                                if (c+cshift < 0 || c+cshift >= width)
                                    cshift *= -1;
                                if (r+rshift < 0 || r+rshift >= height)
                                    rshift *= -1;
                                int img_idx = rand()%FIELDNUM;
                                sampleval(r, c, img_idx) = gray(r+rshift, c+cshift);
                            }
                            else{
                                int index = rand()%FIELDNUM;
                                cshift = FIELD[index][0];
                                rshift = FIELD[index][1];
                                if (c+cshift < 0 || c+cshift >= width)
                                    cshift *= -1;
                                if (r+rshift < 0 || r+rshift >= height)
                                    rshift *= -1;
                                int img_idx = rand()%FIELDNUM;
                                sampleval(r, c, img_idx) = gray(r+rshift, c+cshift);
                            }
                        }

                        segval(r, c) = BACK_GROUND;
                        break;
                    }
                }
            }

            if (count < miniMatch)
                segval(r, c) = FONT_GROUND;
        }
    }
}


pybind11::array_t<unsigned char> VIBE::Vibe(pybind11::array_t<unsigned char> &img_rgb) {
    pybind11::array_t<unsigned char> img_gray = Rgb2Gray(img_rgb);
    pybind11::array_t<unsigned char> segmap = pybind11::array_t<unsigned char>(img_gray.shape()[0] * img_gray.shape()[1]);
    segmap.resize({img_gray.shape()[0], img_gray.shape()[1]});

    if (FIRST) {
        Initial(img_gray);
        FIRST = false;
    }
    else
        Update(img_gray, segmap);
    
    return segmap;
}