===========================
Proposal for Pytorch INT8 GEMM
===========================


Basic information
=================

Pyotrch INT8 convolution and linear layers for both training and inference on NVIDIA GPU to boost your Research and products come true.

Problem to solve
================

Currently, deep learning(DL) frameworks only support FP32/FP16 convolution and linear layers for training and inference. Quantizing DL model to INT8 can speed up training and infrence and decrease power comsumptions, so many studies are proposed. However, they prune to use FP32 to simulate behaviors of quantization because of only tensorflow support INT8 inference. Convolution and linear layers are implemented by GEMM. Hence, this project aims to build custom pytorch INT8 convolution and linear layer for both training inference on NVIDIA GPU.

Based on project Neural networks Training and Inference using Integer-only arithmeetic: https://github.com/wangmaolin/niti
Problems should be solve:
1. Each FP32 tensor is represented by scale, zero-point, and quantized value. After one convolution or linear layer, output tensor is INT32. This output tensor need to quantize to INT8. Quantization involves to rescale the tensor. How to rescale values in tensor efficiently?
2. For convolution layer, how to transofrm correponding matrix to perform convolution operations by GEMM(im2col)?
3. For convolution layer, how to transofrm output of GEMM to correspond ourput of convolution(col2im)?
3. Can we support NVIDIA GPU without Tensore Core?

Perspective users
=================

- Engineers can use our custom plugin on edge devices which have pytorch and NVIDIA GPU. They can use our INT8 layers to perform inference or on-device learning to enhance the models.
- Researchers can base on our INT8 layers to validate their algorithms.

System architecture
===================


API description
===============

Show how a user programmatically uses your system.  You are supposed to
implement the system using both C++ and Python.  Describe how a user writes a
script in the system.  If you provide API in both C++ and Python, describe
both.

Engineering infrastructure
==========================

Schedule
========

References
==========

- Neural networks Training and Inference using Integer-only arithmeetic: https://github.com/wangmaolin/niti
