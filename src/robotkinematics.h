//
// Created by JerryWang on 24-5-15.
//

#ifndef ROBOTSIMULATION_ROBOTKINEMATICS_H
#define ROBOTSIMULATION_ROBOTKINEMATICS_H

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include "kin.h"

using namespace Eigen;
using namespace ur_kinematics;

class RobotKinematics {
public:
    RobotKinematics() = default;

    /// 将变换矩阵转换为xyz和rpy
    void _matrixToXyzRpy(const double *T, double *xyzrpy) {
        // 提取位置
        xyzrpy[0] = T[3];  // x
        xyzrpy[1] = T[7];  // y
        xyzrpy[2] = T[11]; // z

        // 提取旋转矩阵的元素
        double r11 = T[0], r12 = T[1], r13 = T[2];
        double r21 = T[4], r22 = T[5], r23 = T[6];
        double r31 = T[8], r32 = T[9], r33 = T[10];

        // 计算pitch
        double pitch = atan2(-r31, sqrt(r11 * r11 + r21 * r21));

        double roll, yaw;

        if (fabs(pitch) < M_PI / 2 - 1e-6) { // not at singularity
            roll = atan2(r32, r33);
            yaw = atan2(r21, r11);
        } else { // at singularity
            roll = 0;
            yaw = atan2(-r12, r22);
        }

        xyzrpy[3] = roll;  // roll
        xyzrpy[4] = pitch; // pitch
        xyzrpy[5] = yaw;   // yaw
    }

    /// 运动学正解
    /// \param q 6个关节的弧度值
    /// \return T
    double *forwardSolution(double q[6]) {
        double *T = new double[16];
        forward(q, T);
        double *xyzrpy = new double[6];
        _matrixToXyzRpy(T, xyzrpy);
        return xyzrpy;
    }

    void inverseSolution(double *T) {
        double q_sols[8 * 6];
        int num_sols;
        num_sols = inverse(T, q_sols);
        printf("***************************************\n");
        for (int i = 0; i < num_sols; i++)
            printf("%1.6f %1.6f %1.6f %1.6f %1.6f %1.6f\n",
                   q_sols[i * 6 + 0] * (180.0 / M_PI), q_sols[i * 6 + 1] * (180.0 / M_PI),
                   q_sols[i * 6 + 2] * (180.0 / M_PI), q_sols[i * 6 + 3] * (180.0 / M_PI),
                   q_sols[i * 6 + 4] * (180.0 / M_PI),
                   q_sols[i * 6 + 5] * (180.0 / M_PI));
        printf("***************************************\n");
    };

    // 计算绕x轴的旋转矩阵
    void rotationX(double angle, double *R) {
        R[0] = 1;
        R[1] = 0;
        R[2] = 0;
        R[3] = 0;
        R[4] = cos(angle);
        R[5] = -sin(angle);
        R[6] = 0;
        R[7] = sin(angle);
        R[8] = cos(angle);
    }

// 计算绕y轴的旋转矩阵
    void rotationY(double angle, double *R) {
        R[0] = cos(angle);
        R[1] = 0;
        R[2] = sin(angle);
        R[3] = 0;
        R[4] = 1;
        R[5] = 0;
        R[6] = -sin(angle);
        R[7] = 0;
        R[8] = cos(angle);
    }

// 计算绕z轴的旋转矩阵
    void rotationZ(double angle, double *R) {
        R[0] = cos(angle);
        R[1] = -sin(angle);
        R[2] = 0;
        R[3] = sin(angle);
        R[4] = cos(angle);
        R[5] = 0;
        R[6] = 0;
        R[7] = 0;
        R[8] = 1;
    }

// 矩阵乘法 R3x3 = A3x3 * B3x3
    void multiplyMatrix3x3(const double *A, const double *B, double *R) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                R[i * 3 + j] = A[i * 3] * B[j] + A[i * 3 + 1] * B[j + 3] + A[i * 3 + 2] * B[j + 6];
            }
        }
    }

// 将xyz和rpy转换为4x4变换矩阵
    void xyzrpyToMatrix(const double *xyzrpy, double *T) {
        double x = xyzrpy[0];
        double y = xyzrpy[1];
        double z = xyzrpy[2];
        double roll = xyzrpy[3];
        double pitch = xyzrpy[4];
        double yaw = xyzrpy[5];

        // 计算旋转矩阵
        double Rz[9], Ry[9], Rx[9], Rzy[9], R[9];
        rotationZ(yaw, Rz);
        rotationY(pitch, Ry);
        rotationX(roll, Rx);

        multiplyMatrix3x3(Rz, Ry, Rzy);
        multiplyMatrix3x3(Rzy, Rx, R);

        // 构建4x4变换矩阵
        T[0] = R[0];
        T[1] = R[1];
        T[2] = R[2];
        T[3] = x;
        T[4] = R[3];
        T[5] = R[4];
        T[6] = R[5];
        T[7] = y;
        T[8] = R[6];
        T[9] = R[7];
        T[10] = R[8];
        T[11] = z;
        T[12] = 0;
        T[13] = 0;
        T[14] = 0;
        T[15] = 1;
    }

    // 角度规范化函数
    double normalizeAngle(double angle) {
        // 将角度限制在0-360度之间
        angle = fmod(angle, 360.0);
        if (angle < 0) {
            angle += 360.0;
        }

        // 将角度转换到-180到180度之间
        if (angle > 180.0) {
            angle -= 360.0;
        }

        return angle;
    }

    std::vector<std::vector<double>> inverseSolutionFromEulerAngle(double *xyzrpy) {
        double T[16];
        xyzrpyToMatrix(xyzrpy, T);

        double q_sols[8 * 6];
        int num_sols;
        num_sols = inverse(T, q_sols);
//        printf("***************************************\n");
        std::vector<std::vector<double>> res;
        for (int i = 0; i < num_sols; i++) {
            std::vector<double> tmp{
                    normalizeAngle(q_sols[i * 6 + 0] * (180.0 / M_PI)),
                    normalizeAngle(q_sols[i * 6 + 1] * (180.0 / M_PI)),
                    normalizeAngle(q_sols[i * 6 + 2] * (180.0 / M_PI)),
                    normalizeAngle(q_sols[i * 6 + 3] * (180.0 / M_PI)),
                    normalizeAngle(q_sols[i * 6 + 4] * (180.0 / M_PI)),
                    normalizeAngle(q_sols[i * 6 + 5] * (180.0 / M_PI))};
            res.push_back(tmp);
        }
        return res;
    };

};

#endif //ROBOTSIMULATION_ROBOTKINEMATICS_H


