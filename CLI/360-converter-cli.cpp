

#include <iostream>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../example/include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../example/include/stb_image_write.h"

#define CONVERTER_IMPLEMENTATION
#include "../src/converter.hpp"

#ifndef CHANNEL_NUM
#define CHANNEL_NUM 4
#endif

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

char* getCmdOption(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

void printHelp() {
    std::cout << "-----------NEUX-360-CONVERTER----------- \n";
    std::cout << "USAGE \n";
    std::cout << "--help --> shows this message \n";
    std::cout << "--input-{face} {path} --> with face being (front,right,back,left,top,down), Path of each of the 6 needed faces \n";
    std::cout << "--output {path} -> Output Image Path with extension(Only .png supported) \n";
    std::cout << "--type {type} --> with type being (cubemap,equirectangular,stereographic), Method of conversion \n";

}

int main(int argc, char* argv[])
{
    if (cmdOptionExists(argv, argv + argc, "-h") || cmdOptionExists(argv, argv + argc, "--help"))
    {
        printHelp();
        return 0;
    }

    std::string types[3];
    types[0] = "cubemap";
    types[1] = "equirectangular";
    types[2] = "stereographic";
    // = {"cubemap","equirectangular","stereographic"};

    char* imlist[6];
    imlist[0] = getCmdOption(argv, argv + argc, "--input-front");
    imlist[1] = getCmdOption(argv, argv + argc, "--input-right");
    imlist[2] = getCmdOption(argv, argv + argc, "--input-back");
    imlist[3] = getCmdOption(argv, argv + argc, "--input-left");
    imlist[4] = getCmdOption(argv, argv + argc, "--input-top");
    imlist[5] = getCmdOption(argv, argv + argc, "--input-down");

    char* type = getCmdOption(argv, argv + argc, "--type");
    char* output = getCmdOption(argv, argv + argc, "--output");

    if (!imlist[0] || !imlist[1] || !imlist[2] || !imlist[2] || !imlist[3] || !imlist[4] || !imlist[5] || !type || !output) {
        std::cout << "Incorrect Command \n";
        printHelp();
        return 0;

    }
    else {
        Converter::Image img;
        Converter::Image faces[Converter::FACE_NUM];

        int w, h, bpp;
        for (int i = 0; i < Converter::FACE_NUM; ++i)
        {
            faces[i].img = stbi_load(imlist[i], &w, &h, &bpp, CHANNEL_NUM);
            faces[i].h = h, faces[i].w = w;
        }

        Converter::Face face = Converter::Face(faces);

        if (type == types[0]) {
            Converter::Cube cube = face.toCube();
            img = cube.getCubeMap();
        }
        else if (type == types[1]) {
            Converter::Equi equi = face.toEqui();
            img = equi.getEqui();
        }
        else if (type == types[2]) {
            Converter::Equi equi = face.toEqui();
            img = equi.toStereo().getStereo();
        }
        else {
            std::cout << "Incorrect Conversion Type \n";
            printHelp();
        }
        stbi_write_png(output, img.w, img.h, CHANNEL_NUM, img.img, img.w * CHANNEL_NUM);
        std::cout << "Image Generated! \n";
    }
    return 0;
}
