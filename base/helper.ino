float filter_smoothing = .09;

double prev_output = -1.0;
double exp_filter(double input){
  double output;
  if (prev_output - -1.0 < .001){
    prev_output = input;
    output = input;
  }else{
    output = filter_smoothing * input + (1.0 - filter_smoothing) * prev_output;
  }
  prev_output = output;
  return output;
}

double samples[10] = {0};
int current_sample = 0;
double sum_samples(){
  double samp_sum = 0;
  for (int i = 0; i < sizeof(samples)/sizeof(double); i ++){
    samp_sum += samples[i] ;
  }
  return samp_sum;
}

double moving_avg(double input){
  int sample_size = sizeof(samples)/sizeof(double);
  double avg;
  if (current_sample < 100){
    samples[current_sample] = input;
    current_sample++;
    return input;
  }else{
    for (int i = 0; i < sample_size-1; i++){
      samples[i] = samples[i+1];
    }
    samples[sample_size - 1] = input;
    avg = sum_samples() / sample_size;
    
    return avg;
  }
}
