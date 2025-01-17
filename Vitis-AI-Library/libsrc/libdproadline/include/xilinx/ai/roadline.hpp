/*
 * Copyright 2019 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Filename: roadline.hpp
 *
 * Description:
 * This network is used to detecting road line
 *
 * Please refer to document "Xilnx_AI_SDK_User_Guide.pdf" for more details of
 *these APIs.
 */

#pragma once
#include <memory>
#include <opencv2/core.hpp>
#include <xilinx/ai/nnpp/roadline.hpp>

namespace xilinx {
namespace ai {


/**
 * @brief Base class for detecting roadline from an image (cv::Mat).
 *
 * Input is an image (cv::Mat).
 *
 * Output road line type and points maked road line.
 *
 * @note The input image size is 640x480
 *
 * Sample code:
 * @code
  auto det = xilinx::ai::RoadLine::create("vpgnet_pruned_0_99");
  auto image = cv::imread("sample_roadline.jpg");
  //    Mat image;
  //    resize(img, image, Size(640, 480));
  if (image.empty()) {
    cerr << "cannot load " << argv[1] << endl;
    abort();
  }

  vector<int> color1 = {0, 255, 0, 0, 100, 255};
  vector<int> color2 = {0, 0, 255, 0, 100, 255};
  vector<int> color3 = {0, 0, 0, 255, 100, 255};

  RoadLineResult results = det->run(image);
  for (auto &line : results.lines) {
    vector<Point> points_poly = line.points_cluster;
    // for (auto &p : points_poly) {
    //  std::cout << p.x << " " << (int)p.y << std::endl;
    //}
    int type = line.type < 5 ? line.type : 5;
    if (type == 2 && points_poly[0].x < image.rows * 0.5)
      continue;
    cv::polylines(image, points_poly, false,
                  Scalar(color1[type], color2[type], color3[type]), 3, CV_AA,
                  0);
  }
    @endcode
 *
 * Display of the model results:
 * @image latex images/sample_roadline_result.jpg "result image" width=300px
 *
 */
class RoadLine {
public:
  /**
   * @brief Factory function to get an instance of derived classes of class
   * RoadLine.
   *
   * @param model_name String of model name
   * @param need_preprocess  normalize with mean/scale or not, default value is
   * true.
   *
   * @return An instance of RoadLine class.
   */

  static std::unique_ptr<RoadLine> create(const std::string &model_name,
                                          bool need_preprocess = true);

protected:
  explicit RoadLine();
  RoadLine(const RoadLine &) = delete;

public:
  virtual ~RoadLine();

public:
  /**
   * @brief Function to get InputWidth of the roadline network (input image
   * cols).
   *
   * @return InputWidth of the roadline network.
   */
  virtual int getInputWidth() const = 0;
  /**
   * @brief Function to get InputHight of the roadline network (input image
   * rows).
   *
   * @return InputHeight of the roadline network.
   */
  virtual int getInputHeight() const = 0;

  /**
   * @brief Function of get running result of the RoadLine network.
   *
   * @param image Input data , input image (cv::Mat) need to resized as 640x480.
   *
   * @return The struct of RoadLineResult
   */
  virtual RoadLineResult run(const cv::Mat &image) = 0;
};
} // namespace ai
} // namespace xilinx
