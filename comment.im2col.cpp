template <typename Dtype>
void im2col_cpu(const Dtype* data_im, const int channels,
    const int height, const int width, const int kernel_h, const int kernel_w,
    const int pad_h, const int pad_w,
    const int stride_h, const int stride_w,
    const int dilation_h, const int dilation_w,
    Dtype* data_col) {
  const int output_h = (height + 2 * pad_h -
    (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  const int output_w = (width + 2 * pad_w -
    (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  const int channel_size = height * width;// data_im的步长,用于1重for loop
  for (int channel = channels; channel--; data_im += channel_size) {// channel仅控制循环次数
    for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {// kernel_row决定input_row的初值
      for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {// kernel_col决定input_col的初值
        int input_row = -pad_h + kernel_row * dilation_h;//input_row与height作为is_a_ge_zero_and_a_lt_b参数//在这里找到卷积核中的某一行在输入图像中的第一个操作区域的行索引
		/*第四个和第五个for循环表示了输出单通道矩阵的某一行，同时体现了输出单通道矩阵的列数*/
        for (int output_rows = output_h; output_rows; output_rows--) {// output_rows仅控制循环次数,决定input_row+=stride_h执行次数
          if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {// 1-4重for循环都是无分支,5重for循环有分支进行不同的for循环 //如果计算得到的输入图像的行值索引小于零或者大于输入图像的高(该行为pad)
            for (int output_cols = output_w; output_cols; output_cols--) {// output_cols仅控制循环次数，不参与计算
              *(data_col++) = 0;//那么将该行在输出的矩阵上的位置置为0
            }//5层for循环，output_cols控制
          } else {// if(!is_a_ge_zero_and_a_lt_b(input_row, height)) else()
            int input_col = -pad_w + kernel_col * dilation_w;//在这里找到卷积核中的某一列在输入图像中的第一个操作区域的列索引
            for (int output_col = output_w; output_col; output_col--) {
            //上面由kernel_col确实input_col的一个初始值，在我这个循环内, 循环末句通过input_col += stride_w改变
              if (is_a_ge_zero_and_a_lt_b(input_col, width)) {//如果计算得到的输入图像的列值索引大于等于于零或者小于输入图像的宽(该列不是pad)
                *(data_col++) = data_im[input_row * width + input_col];//将输入特征图上对应的区域放到输出矩阵上
              } else {//否则，计算得到的输入图像的列值索引小于零或者大于输入图像的宽(该列为pad)
                *(data_col++) = 0;//将该行该列在输出矩阵上的位置置为0
              }
              input_col += stride_w;//按照宽方向步长遍历卷积核上固定列在输入图像上滑动操作的区域
            }//5层for循环，output_col控制
          }// !is_a_ge_zero_and_a_lt_b(input_row, height)
          input_row += stride_h;//按照高方向步长遍历卷积核上固定行在输入图像上滑动操作的区域
        }//4层for循环，output_rows控制
      }//3层for循环,kernel_col控制
    }//2层for循环,kernel_row控制
  }//1层for循环,每次完成后在for()内data_im+=channel_size,依次处理data_im的channel0, channel1, channel2, ..., channel_(Cin-1)
}// im2col_cpu



template <typename Dtype>
void col2im_cpu(const Dtype* data_col, const int channels,
    const int height, const int width, const int kernel_h, const int kernel_w,
    const int pad_h, const int pad_w,
    const int stride_h, const int stride_w,
    const int dilation_h, const int dilation_w,
    Dtype* data_im) {
  caffe_set(height * width * channels, Dtype(0), data_im);
  const int output_h = (height + 2 * pad_h -
    (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  const int output_w = (width + 2 * pad_w -
    (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  const int channel_size = height * width;
  for (int channel = channels; channel--; data_im += channel_size) {// channel仅控制循环次数
    for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {// kernel_row决定input_row的初值
      for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {// kernel_col决定input_col的初值
        int input_row = -pad_h + kernel_row * dilation_h;//input_row与height作为is_a_ge_zero_and_a_lt_b参数//在这里找到卷积核中的某一行在输入图像中的第一个操作区域的行索引
		/*第四个和第五个for循环表示了输出单通道矩阵的某一行，同时体现了输出单通道矩阵的列数*/
        for (int output_rows = output_h; output_rows; output_rows--) {//output_rows仅控制循环次数,决定input_row+=stride_h执行次数 
          if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
            data_col += output_w;
          } else {
            int input_col = -pad_w + kernel_col * dilation_w;
            for (int output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
                data_im[input_row * width + input_col] += *data_col;
              }
              data_col++;
              input_col += stride_w;
            }//5层循环, output_col控制
          }
          input_row += stride_h;
        }//4层循环，output_rows控制
      }//3层循环,kernel_col控制
    }//2层循环,kernel_row控制
  }// 1层循环, channel控制
}

