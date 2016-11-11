/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
  #include "openag_dfr0300_ds18b20.h"
  
  
  
  
  
  
  
  bool Dfr0300_Ds18b20::get_water_electrical_conductivity(std_msgs::Float32 &msg){
   msg.data = _water_electrical_conductivity;
   bool res = _send_water_electrical_conductivity;
   _send_water_electrical_conductivity = false;
   return res;
 }
 
 bool Dfr0300_Ds18b20::get_water_temperature(std_msgs::Float32 &msg) {
  msg.data = _water_temperature;
  bool res = _send_water_temperature;
  _send_water_temperature = false;
  return res;
 }
 //.......................................Private.......................................//
 
  float Dfr0300_Ds18b20::getWT(void){
  _sensors.requestTemperatures();
 }
 
 
 float Dfr0300_Ds18b20::getWEC(void){
     int analog_sum = 0;
   const int samples = 40;
   _send_water_electrical_conductivity = true;
   for (int i = 0; i<samples; i++){
     analog_sum += analogRead(_ec_pin);
     delay(2);
   }
   float analog_average = (float) analog_sum / samples;
   float analog_voltage = analog_average*(float)5000/1024;
   float temperature_coefficient = 1.0 + 0.0185*(23.5 - 25.0);
   float voltage_coefficient = analog_voltage / temperature_coefficient;
   if(voltage_coefficient < 0) {
    _water_electrical_conductivity = 0;
    //Serial.println("No solution!");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
  }
  else if (voltage_coefficient > 3300) {
    Serial2.println("Out");
    _water_electrical_conductivity = 0;
    //Serial.println("Out of the range!");  //>20ms/cm,out of the range
  }
  else { 
    if(voltage_coefficient <= 448) {
      _water_electrical_conductivity = (6.84*voltage_coefficient-64.32)/1000 + _ec_calibration_offset;
      /*Serial2.print("448: ");
      Serial2.println(_water_electrical_conductivity); 
      return (_water_electrical_conductivity);   //1ms/cm<EC<=3ms/cm*/
    }
    else if (voltage_coefficient <= 1457) {
      _water_electrical_conductivity = (6.98*voltage_coefficient-127)/1000 + _ec_calibration_offset;
      /*Serial2.print("1457: ");
      Serial2.println(_water_electrical_conductivity);
      return (_water_electrical_conductivity);  //3ms/cm<EC<=10ms/cm*/
    }
    else {
      _water_electrical_conductivity = (5.3*voltage_coefficient+2278)/1000 + _ec_calibration_offset;
      /*Serial2.print("else: ");
      Serial2.println(_water_electrical_conductivity); 
      return (_water_electrical_conductivity); //10ms/cm<EC<20ms/cm*/
    }
   }
   return (_water_electrical_conductivity);
 }
 
 }
   
 