
#include <string>
#include <vector>
#include "tat_utils.h"

enum NeuronType {  eIntrinsically_bursting,eChattering,eFast_spiking,
  eLow_threshold,eResonator,eRegular_spiking};




class SpikingUnit {

    int population_size=1;
  bool mdebug = false;
  float max_threshold = 65.0;
  int substeps;
  NeuronType neuronType;
  float tau_recovery;
  float coupling;
  float reset_voltage;
  float reset_recovery;
  float dir_current;
  float voltage;
  float recovery;
  FloatList* e_synapse;
  FloatList* i_synapse;
  std::string name;
  float noisefact = 0;
  
  //float[] vlt_buffer;
  public:
  // constructor
  SpikingUnit();
  SpikingUnit(std::string aname, NeuronType a_ntype, int a_substeps);
  SpikingUnit(std::string aname, NeuronType a_ntype, int a_substeps, float noise);
  ~SpikingUnit();
  
  // accessors
  float getVlt();
  float getOutput();
  float getNormOutput();
  float getPhase();
  float getResetVoltage();
  float getTauRecovery();
  float getCoupling();
  float getResetRecovery();
  
  void setDirect(float current);
  void setDbg(bool on);
  void modulateResetVoltage(float val);
  void setResetVoltage(float val);
  void setNeuronType(NeuronType a_ntype);

  /** Randomization as per Izhikevich 2003
  */
  void randomize();
  
  void reset();
  
  void excite(float val);
  void inhibit(float val);
  
  
  
  void tick();
  
  // visualize
     
  FloatList timeStep_Iz(             float a_a, // tau recovery   //<>//
                                float a_b, // coupling
                                float a_c, // reset voltage
                                float a_d, // reset recovery    //<>//
                                FloatList *e_syn, // synapse - contains excitation and inh scaled by synaptic vals, or zero if not spiked    //<>//
                                FloatList *i_syn,   //<>//
                                float a_i, // direct current   //<>//
                                float a_v, // in out excitation
                                float a_u  // recovery  
                                );   
  
  
  void debug();
};

/*
Spiking population with connection topology
*/

class SpikingPopulation{
 
  std::vector<SpikingUnit*> units;
  std::vector<Buffer*> data;
  FloatList output;
  float** internal_synapse;
  int syn_x;
  int syn_y;
  std::string name;
  
  public:
  // constructor
  SpikingPopulation();
  SpikingPopulation(std::string aname, int sz, NeuronType ntype, int bufsize);
  SpikingPopulation(std::string aname, int sz, NeuronType ntype, int bufsize, float noise);
  ~SpikingPopulation();
  
  // accessors
  std::string getName();
  int getSize();
  FloatList &getOutput();
  FloatList &getNormOutput();
  std::vector<Buffer*> &getBuffers();
  void reset();
  void setInternalTopology(float** top, int sx, int sy);
    
  
  // methods
  void tick();
  void setDirect(FloatList &val);
  void setResetVoltage(float val);
  void excite(FloatList &val, float** synapse, int sx, int sy);
  void inhibit(FloatList &val, float** synapse, int sx, int sy);
  void modulateResetVoltage(float diff);
  void modulateResetVoltage(FloatList &diff);
  void randomize();
  
  // privates
  private:
  void updateConn(float** matrix, std::vector<SpikingUnit*> &units, int sx, int sy);
};