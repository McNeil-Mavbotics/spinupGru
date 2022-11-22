class PIDController {
  public:
    double kp;
    double ki;
    double kd;
    
    double elapsedTime = 0.02;
    double error;
    double lastError;
    double setPoint = 0;
    double sumError, rateError;
    
    void setUp(double kpIn, double kiIn, double kdIn) {
      kp = kpIn;
      ki = kiIn;
      kd = kdIn;
    }

    void changeTarget(double setPointValue){
      setPoint = setPointValue;
    }    
    
    void changeValues(double input){
      error = setPoint - input;
      sumError += error * elapsedTime;
      lastError = error;
    }

    double getTarget() {
      return setPoint;
    }
    
    double computePID(double input){
      error = setPoint - input;
      sumError += error * elapsedTime;
      rateError = (error - lastError) / elapsedTime;

      double out = kp * error + ki * sumError + kd * rateError;           

      lastError = error;
      return out;
    }
};