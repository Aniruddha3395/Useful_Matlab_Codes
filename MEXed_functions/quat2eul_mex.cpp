// Author    : Aniruddha Shembekar, Research Engineer, University of Southern California

#include "mex.h"
#include "matrix.h"
#include <iostream>
#include "string.h"
#include </usr/local/include/eigen3/Eigen/Eigen>
#include <stdio.h>
#include <vector>

Eigen::MatrixXd qt2eul(Eigen::MatrixXd, std::string);
Eigen::Matrix3d qt2rot(Eigen::MatrixXd);
std::string validate_seq(std::string);

void mexFunction (int _OutArgs, mxArray *MatlabOut[], int _InArgs, const mxArray *MatlabIn[] )
{
    if (_InArgs==2) 
    {
        // Define Input
        std::string seq_in = "";
        Eigen::Map<Eigen::ArrayXXd,Eigen::Aligned> quat (mxGetPr(MatlabIn[0]), 1, 4); 
        if (mxIsChar(MatlabIn[1])==1)
        {
           char* pCharArray = mxArrayToString(MatlabIn[1]); 
           std::string str(pCharArray);
           seq_in = str;    
        }
        
        // Method 
        Eigen::Matrix3d eul_angles = qt2eul(quat, seq_in);
        
        // Define Output
        MatlabOut[0] = mxCreateDoubleMatrix(1,3,mxREAL);
        Eigen::Map<Eigen::ArrayXXd,Eigen::Aligned> M0 (mxGetPr(MatlabOut[0]),1,3);
        M0 = eul_angles.array(); 
    }
    else
    {
        // Define Input
        Eigen::Map<Eigen::ArrayXXd,Eigen::Aligned> quat (mxGetPr(MatlabIn[0]), 1, 4); 
        
        // Method 
        std::string seq_in = "";
        Eigen::Matrix3d eul_angles = qt2eul(quat, seq_in);
        
        // Define Output
        MatlabOut[0] = mxCreateDoubleMatrix(1,3,mxREAL);
        Eigen::Map<Eigen::ArrayXXd,Eigen::Aligned> M0 (mxGetPr(MatlabOut[0]),1,3);
        M0 = eul_angles.array();    
    }
}

Eigen::MatrixXd qt2eul(Eigen::MatrixXd quat, std::string seq)
{
    seq = validate_seq(seq);
    Eigen::Matrix3d rot_mat = qt2rot(quat);
    int rot_idx[3];
    for (int i=0; i<3; ++i)
    {
        if(seq[i]=='X' || seq[i]=='x')
            rot_idx[i] = 0;
        else if(seq[i]=='Y' || seq[i]=='y')
            rot_idx[i] = 1;
        else if(seq[i]=='Z' || seq[i]=='z')
            rot_idx[i] = 2;
    }   
    Eigen::MatrixXd eul_angles(1,3);
    Eigen::Vector3d eul_angles_vec;
    eul_angles_vec = rot_mat.eulerAngles(rot_idx[0], rot_idx[1], rot_idx[2]);
    eul_angles(0,0) = eul_angles_vec[0];
    eul_angles(0,1) = eul_angles_vec[1];
    eul_angles(0,2) = eul_angles_vec[2];    
    return eul_angles;
}

Eigen::Matrix3d qt2rot(Eigen::MatrixXd quat)
{
    Eigen::Quaterniond q;
    q.x() = quat(0,0);
    q.y() = quat(0,1);
    q.z() = quat(0,2);
    q.w() = quat(0,3);  
    return q.normalized().toRotationMatrix();
}

std::string validate_seq(std::string seq)
{
    if(seq =="")
        seq = "ZYX";    
    bool invalid_flag = false;
    if(seq.size()!=3)
    {
        invalid_flag = true;
    }
    for (int i =0;i<3;++i)
    {
        if(seq[i]!='X' && seq[i]!='Y' && seq[i]!='Z' && seq[i]!='x' && seq[i]!='y' && seq[i]!='z')
        {
            invalid_flag = true; 
            break;
        }
    }
    if(invalid_flag)
    {
        std::cerr << "ERROR: Invalid Rotations Sequence: returning the value in 'ZYX' sequence " << seq << std::endl;
        return "ZYX";       
    }
    return seq;
}