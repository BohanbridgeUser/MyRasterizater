#include <iostream>

#include <opencv2/opencv.hpp>
#include <Eigen/Eigen>
#include <fstream>
#include <ctime>

#include "define.h"
#include "model/OBJ_Import.h"
#include "rasterizer/rasterizer.h"
#include "window/Mywindow.h"
#include "VertexBuffer.h"
#include "Texture.h"

int main()
{
    std::clock_t begin=clock();
    /*OBJ::OBJ_Importer model("res/models/triangle/triangle2.obj");*/
    /*OBJ::OBJ_Importer model("res/models/cube/cube.obj");*/
    /*OBJ::OBJ_Importer model("res/models/bunny/bunny.obj");*/
    OBJ::OBJ_Importer model("res/models/spot/spot_triangulated_good.obj");
    model.Import();
    std::clock_t end = clock();
    std::cout << "Model Input : " << (end - begin) / CLOCKS_PER_SEC << std::endl;

    begin = clock();

    MyRasterizater::Rasterizter rasterizter(700, 700);
    rasterizter.SetVBO(model);
    rasterizter.SetIBO(model);
    rasterizter.InputTexture("res/models/spot/spot_texture.png");
    /*rasterizter.InputTexture("res/models/cube/wall.tif");*/
    MyRasterizater::Light light1(OBJ::Vector3f(20.0f,20.0f,20.0f), OBJ::Vector3f(500.00f,500.00f, 500.00f));
    MyRasterizater::Light light2(OBJ::Vector3f(-20.0f, 20.0f, 0.00f), OBJ::Vector3f(500.00f, 500.00f, 500.00f));
    rasterizter.PushLight(light1);
    rasterizter.PushLight(light2);
    rasterizter.SetCamera(OBJ::Vector3f(0, 0, 10.0f));

    glm::mat4 viewm = rasterizter.translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-10.0f));

    glm::mat4 pesproj = rasterizter.perspective(glm::radians(45.00f), (700.00 / 700.00), 0.1f, 50.00f);

    glm::mat4 scaler = rasterizter.scale(glm::mat4(1.00f), glm::vec3(2.5f, 2.5f, 2.5f));

    glm::mat4 rotater = rasterizter.rotation(glm::mat4(1.00f),glm::radians(60.00f), glm::vec3(0.00f, 1.00f, 0.00f));
    rasterizter.SetModelm(rotater * scaler);
    rasterizter.SetViewm(viewm);
    rasterizter.SetProjm(pesproj);
    /*rasterizter.SetMVP(pesproj * scaler * modelm);*/
    end = clock();
    std::cout << "Rasterizter Preparation : " << (end - begin) / CLOCKS_PER_SEC << std::endl;
    
    begin = clock();
    rasterizter.Draw();
    Mywindow window("Rasterizter");
    cv::Mat image(700.00f, 700.00f, CV_32FC3, rasterizter.GetFrameBuffer());
    end = clock();
    std::cout << "Rasterizting : " << (end - begin) / CLOCKS_PER_SEC << std::endl;
    
    /*image.convertTo(image, CV_8UC3, 1.0f);*/

    window.SetImage(image);
    window.Begin();
    return 0;
}