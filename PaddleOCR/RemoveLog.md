# 去掉PaddleOCR C++的输出

在下面五个文件的LoadModel函数的paddle_infer::Config config;后面添加config.DisableGlogInfo();，然后把函数最后面的DisableGlogInfo();去掉。这时log就没了，具体就是：





在ocr_rec.cpp的132行的LoadModel改成：

```C++
void CRNNRecognizer::LoadModel(const std::string &model_dir) {
  paddle_infer::Config config;
  config.DisableGlogInfo();
  config.SetModel(model_dir + "/inference.pdmodel",
                  model_dir + "/inference.pdiparams");
  //std::cout << "In PP-OCRv3, default rec_img_h is 48,"
  //          << "if you use other model, you should set the param rec_img_h=32"
  //          << std::endl;
  if (this->use_gpu_) {
    config.EnableUseGpu(this->gpu_mem_, this->gpu_id_);
    if (this->use_tensorrt_) {
      auto precision = paddle_infer::Config::Precision::kFloat32;
      if (this->precision_ == "fp16") {
        precision = paddle_infer::Config::Precision::kHalf;
      }
      if (this->precision_ == "int8") {
        precision = paddle_infer::Config::Precision::kInt8;
      }
      if (!Utility::PathExists("./trt_rec_shape.txt")) {
        config.CollectShapeRangeInfo("./trt_rec_shape.txt");
      } else {
        config.EnableTunedTensorRtDynamicShape("./trt_rec_shape.txt", true);
      }
    }
  } else {
    config.DisableGpu();
    if (this->use_mkldnn_) {
      config.EnableMKLDNN();
      // cache 10 different shapes for mkldnn to avoid memory leak
      config.SetMkldnnCacheCapacity(10);
    } else {
      config.DisableMKLDNN();
    }
    config.SetCpuMathLibraryNumThreads(this->cpu_math_library_num_threads_);
  }

  // get pass_builder object
  auto pass_builder = config.pass_builder();
  // delete "matmul_transpose_reshape_fuse_pass"
  pass_builder->DeletePass("matmul_transpose_reshape_fuse_pass");
  config.SwitchUseFeedFetchOps(false);
  // true for multiple input
  config.SwitchSpecifyInputNames(true);

  config.SwitchIrOptim(true);

  config.EnableMemoryOptim();
  //   config.DisableGlogInfo();

  this->predictor_ = paddle_infer::CreatePredictor(config);
}
```

在ocr_cls.cpp的104行的LoadModel改成：

```C++
void Classifier::LoadModel(const std::string &model_dir) {
  paddle_infer::Config config;
  config.DisableGlogInfo();
  config.SetModel(model_dir + "/inference.pdmodel",
                  model_dir + "/inference.pdiparams");

  if (this->use_gpu_) {
    config.EnableUseGpu(this->gpu_mem_, this->gpu_id_);
    if (this->use_tensorrt_) {
      auto precision = paddle_infer::Config::Precision::kFloat32;
      if (this->precision_ == "fp16") {
        precision = paddle_infer::Config::Precision::kHalf;
      }
      if (this->precision_ == "int8") {
        precision = paddle_infer::Config::Precision::kInt8;
      }
      config.EnableTensorRtEngine(1 << 20, 10, 3, precision, false, false);
      //if (!Utility::PathExists("./trt_cls_shape.txt")) {
      //  config.CollectShapeRangeInfo("./trt_cls_shape.txt");
      //} else {
      //  config.EnableTunedTensorRtDynamicShape("./trt_cls_shape.txt", true);
      //}
    }
  } else {
    config.DisableGpu();
    if (this->use_mkldnn_) {
      config.EnableMKLDNN();
    } else {
      config.DisableMKLDNN();
    }
    config.SetCpuMathLibraryNumThreads(this->cpu_math_library_num_threads_);
  }

  // false for zero copy tensor
  config.SwitchUseFeedFetchOps(false);
  // true for multiple input
  config.SwitchSpecifyInputNames(true);

  config.SwitchIrOptim(true);

  config.EnableMemoryOptim();
  //config.DisableGlogInfo();

  this->predictor_ = paddle_infer::CreatePredictor(config);
}
```

在ocr_det.cpp的19行的LoadModel改成：

```C++
void DBDetector::LoadModel(const std::string &model_dir) {
  //   AnalysisConfig config;
  paddle_infer::Config config;
  config.DisableGlogInfo();
  config.SetModel(model_dir + "/inference.pdmodel",
                  model_dir + "/inference.pdiparams");

  if (this->use_gpu_) {
    config.EnableUseGpu(this->gpu_mem_, this->gpu_id_);
    if (this->use_tensorrt_) {
      auto precision = paddle_infer::Config::Precision::kFloat32;
      if (this->precision_ == "fp16") {
        precision = paddle_infer::Config::Precision::kHalf;
      }
      if (this->precision_ == "int8") {
        precision = paddle_infer::Config::Precision::kInt8;
      }
      config.EnableTensorRtEngine(1 << 30, 1, 20, precision, false, false);
      //if (!Utility::PathExists("./trt_det_shape.txt")) {
      //  config.CollectShapeRangeInfo("./trt_det_shape.txt");
      //} else {
      //  config.EnableTunedTensorRtDynamicShape("./trt_det_shape.txt", true);
      //}
    }
  } else {
    config.DisableGpu();
    if (this->use_mkldnn_) {
      config.EnableMKLDNN();
      // cache 10 different shapes for mkldnn to avoid memory leak
      config.SetMkldnnCacheCapacity(10);
    } else {
      config.DisableMKLDNN();
    }
    config.SetCpuMathLibraryNumThreads(this->cpu_math_library_num_threads_);
  }
  // use zero_copy_run as default
  config.SwitchUseFeedFetchOps(false);
  // true for multiple input
  config.SwitchSpecifyInputNames(true);

  config.SwitchIrOptim(true);

  config.EnableMemoryOptim();
  // config.DisableGlogInfo();

  this->predictor_ = paddle_infer::CreatePredictor(config);
}
```

在structure_layout.cpp的96行的LoadModel改成：

```C++
void StructureLayoutRecognizer::LoadModel(const std::string &model_dir) {
  paddle_infer::Config config;
  config.DisableGlogInfo();
  if (Utility::PathExists(model_dir + "/inference.pdmodel") &&
      Utility::PathExists(model_dir + "/inference.pdiparams")) {
    config.SetModel(model_dir + "/inference.pdmodel",
                    model_dir + "/inference.pdiparams");
  } else if (Utility::PathExists(model_dir + "/model.pdmodel") &&
             Utility::PathExists(model_dir + "/model.pdiparams")) {
    config.SetModel(model_dir + "/model.pdmodel",
                    model_dir + "/model.pdiparams");
  } else {
    std::cerr << "[ERROR] not find model.pdiparams or inference.pdiparams in "
              << model_dir << std::endl;
    exit(1);
  }

  if (this->use_gpu_) {
    config.EnableUseGpu(this->gpu_mem_, this->gpu_id_);
    if (this->use_tensorrt_) {
      auto precision = paddle_infer::Config::Precision::kFloat32;
      if (this->precision_ == "fp16") {
        precision = paddle_infer::Config::Precision::kHalf;
      }
      if (this->precision_ == "int8") {
        precision = paddle_infer::Config::Precision::kInt8;
      }
      config.EnableTensorRtEngine(1 << 20, 10, 3, precision, false, false);
      //if (!Utility::PathExists("./trt_layout_shape.txt")) {
      //  config.CollectShapeRangeInfo("./trt_layout_shape.txt");
      //} else {
      //  config.EnableTunedTensorRtDynamicShape("./trt_layout_shape.txt", true);
      //}
    }
  } else {
    config.DisableGpu();
    if (this->use_mkldnn_) {
      config.EnableMKLDNN();
    } else {
      config.DisableMKLDNN();
    }
    config.SetCpuMathLibraryNumThreads(this->cpu_math_library_num_threads_);
  }

  // false for zero copy tensor
  config.SwitchUseFeedFetchOps(false);
  // true for multiple input
  config.SwitchSpecifyInputNames(true);

  config.SwitchIrOptim(true);

  config.EnableMemoryOptim();
  //config.DisableGlogInfo();

  this->predictor_ = paddle_infer::CreatePredictor(config);
}
```

在structure_table.cpp的120行的LoadModel改成：

```C++
void StructureTableRecognizer::LoadModel(const std::string &model_dir) {
  paddle_infer::Config config;
  config.DisableGlogInfo();
  config.SetModel(model_dir + "/inference.pdmodel",
                  model_dir + "/inference.pdiparams");

  if (this->use_gpu_) {
    config.EnableUseGpu(this->gpu_mem_, this->gpu_id_);
    if (this->use_tensorrt_) {
      auto precision = paddle_infer::Config::Precision::kFloat32;
      if (this->precision_ == "fp16") {
        precision = paddle_infer::Config::Precision::kHalf;
      }
      if (this->precision_ == "int8") {
        precision = paddle_infer::Config::Precision::kInt8;
      }
      config.EnableTensorRtEngine(1 << 20, 10, 3, precision, false, false);
      //if (!Utility::PathExists("./trt_table_shape.txt")) {
      //  config.CollectShapeRangeInfo("./trt_table_shape.txt");
      //} else {
      //  config.EnableTunedTensorRtDynamicShape("./trt_table_shape.txt", true);
      //}
    }
  } else {
    config.DisableGpu();
    if (this->use_mkldnn_) {
      config.EnableMKLDNN();
    } else {
      config.DisableMKLDNN();
    }
    config.SetCpuMathLibraryNumThreads(this->cpu_math_library_num_threads_);
  }
}
```
