  #include "tat_neurosim.h"
  #include "tat_math.h"
#include "assert.h"

  
  
  
  //float[] vlt_buffer;
  // constructor
  SpikingUnit::SpikingUnit(){
    name="unit";
    substeps = 2;
    e_synapse = new FloatList();
    i_synapse = new FloatList();
    noisefact = 0;
    setNeuronType(NeuronType::eRegular_spiking);
  }

  SpikingUnit::SpikingUnit(std::string aname, NeuronType a_ntype,
              int a_substeps){
    name = aname;            
    substeps = a_substeps;
    e_synapse = new FloatList();
    i_synapse = new FloatList();
    noisefact = 0;
    // vlt_buffer = new float[a_buffersz];
    setNeuronType(a_ntype);
   
  }
  
  
  SpikingUnit::SpikingUnit(std::string aname, NeuronType a_ntype, int a_substeps, float noise){
    name = aname;            
    substeps = a_substeps;
    e_synapse = new FloatList();
    i_synapse = new FloatList();
    noisefact = noise;
    // vlt_buffer = new float[a_buffersz];
    setNeuronType(a_ntype);
    randomize();
  }
  
  SpikingUnit::~SpikingUnit() {
    e_synapse->clear();
    i_synapse->clear();
    delete e_synapse;
    delete i_synapse;
  }
  // accessors
  float SpikingUnit::getVlt(){ return voltage; }
  float SpikingUnit::getOutput(){return voltage;}
  float SpikingUnit::getNormOutput(){return voltage >= max_threshold? 1.f : 0.f; }
  float SpikingUnit::getPhase(){return map(voltage, reset_voltage, max_threshold, 0, TWO_PI);}
  float SpikingUnit::getResetVoltage(){return reset_voltage;}
  float SpikingUnit::getTauRecovery(){return tau_recovery;}
  float SpikingUnit::getCoupling(){return coupling;}
  float SpikingUnit::getResetRecovery(){return reset_recovery;}
  
  void SpikingUnit::setDirect(float current){ dir_current = current; }
  void SpikingUnit::setDbg(bool on){ mdebug = on; }
  void SpikingUnit::modulateResetVoltage(float val){reset_voltage += val;}
  void SpikingUnit::setResetVoltage(float val){reset_voltage = val;}
  void SpikingUnit::setNeuronType(NeuronType a_ntype){
     
     switch(a_ntype){
      case eIntrinsically_bursting:
          tau_recovery = 0.02f;
          coupling = 0.2f;
          reset_voltage = -55;
          reset_recovery = 4;
          break;
      case eChattering:
          tau_recovery = 0.02f;
          coupling = 0.2f;
          reset_voltage = -50;
          reset_recovery = 2;
          break;
      case eFast_spiking:
          
          tau_recovery = 0.02f;
          coupling = 0.25f;
          reset_voltage = -65;
          reset_recovery = 2;

          break;
      case eLow_threshold:
          tau_recovery = 0.1f;
          coupling = 0.25f;
          reset_voltage = -65.f;
          reset_recovery = 2.f;

          break;
      case eResonator:
          tau_recovery = 0.1f;
          coupling = 0.26f;
          reset_voltage = -65.f;
          reset_recovery = 2;

          break;
      default:
      case eRegular_spiking:
          tau_recovery = 0.02f;
          coupling = 0.2f;
          reset_voltage = -65.f;
          reset_recovery = 8;
          break;

    }
    neuronType = a_ntype;
  }

  /** Randomization as per Izhikevich 2003
  */
  void SpikingUnit::randomize(){

    float rand = noisefact * random(0, 1);
    switch(neuronType){ //<>//
      case eIntrinsically_bursting: //<>//
          tau_recovery = 0.02f;
          coupling = 0.2f;
          reset_voltage = -55; //<>//
          reset_recovery = 4; //<>//
          break; //<>//
      case eChattering:
          tau_recovery = 0.02f;
          coupling = 0.2f; //<>//
          reset_voltage = -50; //<>//
          reset_recovery = 2; //<>//
          break; //<>//
      case eFast_spiking:
          tau_recovery = 0.02 + 0.08*rand;
          coupling = 0.25f - 0.05*rand;
          //reset_voltage = -65;
          //reset_recovery = 2;

          break;
      case eLow_threshold:
          tau_recovery = 0.1f;
          coupling = 0.25f;
          reset_voltage = -65.f;
          reset_recovery = 2.f;

          break;
      case eResonator:
          tau_recovery = 0.1f;
          coupling = 0.26f;
          reset_voltage = -65.f;
          reset_recovery = 2;

          break;
      default:
      case eRegular_spiking:
          //tau_recovery = 0.02f;
          //coupling = 0.2f;
          reset_voltage = -65.f + 15.f*rand*rand;
          reset_recovery = 8.f - 6.f*rand*rand;
          break;

    }
  }
  
  void SpikingUnit::reset(){
    voltage = 0;
    recovery = 0;
  }
  
  void SpikingUnit::excite(float val){e_synapse->push_back(val);}
  void SpikingUnit::inhibit(float val){i_synapse->push_back(val);}
  
  
  
  void SpikingUnit::tick(){
    FloatList ret = timeStep_Iz(  tau_recovery,
                  coupling,
                  reset_voltage,
                  reset_recovery,
                  e_synapse,
                  i_synapse,
                  dir_current,
                  voltage,
                  recovery);
     voltage = ret[0];
     recovery = ret[1];
     e_synapse->clear();
     i_synapse->clear();   //<>//
  }   //<>//
  
  // visualize
  FloatList SpikingUnit::timeStep_Iz(             float a_a, // tau recovery   //<>//
                                float a_b, // coupling
                                float a_c, // reset voltage
                                float a_d, // reset recovery    //<>//
                                FloatList *e_syn, // synapse - contains excitation and inh scaled by synaptic vals, or zero if not spiked    //<>//
                                FloatList *i_syn,   //<>//
                                float a_i, // direct current   //<>//
                                float a_v, // in out excitation
                                float a_u  // recovery  
                                )   
  {   
    float fired = 0;  
    //for(int i=0; i < population_size; ++i) 
    if(a_v >= max_threshold)
            fired = 1.f;
            //numfired += 1;
    // numfired = sum(fired, population_size); // used for adenosine calc
    
    float v1 = a_v; 
    float u1 = a_u;
    float i1 = 0;
    //int synsize_x = excitation_size + inhibition_size;
    // float **syn_fired = create_matrix(synsize_x, population_size);

    //float *tmp = create_array(population_size);
    //sum(tmp, a_syn, synsize_x, population_size, 1); // sum along x axis

    //for(int j = 0; j < population_size; j++)
    //{
    if(fired == 1.f)
    {
        a_v = max_threshold;
        v1 = a_c;
        u1 = a_u + a_d;
    }
    float inputvlt = 0;
    for(int i=0; i<e_syn->size(); i++){
      if(e_syn->at(i) >= max_threshold )
        inputvlt += e_syn->at(i);
    }
    for(int i=0; i<i_syn->size(); i++){
      if(i_syn->at(i) >= max_threshold)
        inputvlt -= i_syn->at(i);
    }
        // sum up 
        // for(size_t i = 0; i < excitation_size + inhibition_size; i++)
        // {
        //     // TODO: change so inhibition is not removed
        //     if(a_syn[j] >= threshold)
        //         syn_fired[j] = a_syn[j];
        //     
        // }
    //}
    //float tmp = 0;//create_array(population_size);
    //sum(tmp, syn_fired, synsize_x, population_size, 1); // sum along x axis
    //sum(tmp, a_syn, synsize_x, population_size, 1); // sum along x axis
    //Stream<float> fs = Arrays.stream(a_syn); 
    //tmp = 
    i1 = inputvlt + a_i;
    //add(i1, tmp, a_i, population_size ); // add direct current
    
    // calculate output voltage
    float stepfact = 1.f/substeps;
    for(int step = 0; step < substeps; step++)
    {
      // v1=v1+(1.0/substeps)*(0.04*(v1**2)+(5*v1)+140-u + i1) 
      // 
      v1 += stepfact*(0.04*v1*v1 + 5*v1 + 
                  140-u1 + i1);
      // u1=u1+(1.0/substeps)*a*(b*v1-u1)               
    }
    u1 += (a_a*(a_b*v1 - u1));
    // multiply(v1, 100.f, population_size);
    // clip at threshold
    //for(int i = 0; i < population_size; i++)
    //{
    if(v1 >= max_threshold)
        v1 = max_threshold;
    //}
    if(mdebug)
    {
      println();
      println("-- " + name);
      println("inputvlt: " + std::to_string(inputvlt));
      print_array("exc syn_fired: ", e_syn);
      print_array("inh syn_fired: ", i_syn);
      println("vlt: " + std::to_string(voltage));
      println("i1: " + std::to_string(i1));
      
    }
    
    FloatList retval; // = new float[2];
    retval.push_back(v1);
    retval.push_back(u1); 
    //copy_array(a_v, v1, population_size); // output voltage
    //copy_array(a_u, u1, population_size); // output recovery

    // destroy_matrix(syn_fired);
    return retval;
        
  }
  
  void SpikingUnit::debug(){
    println();
    println("-- " + name + " --");
    println ("tau: " + std::to_string(tau_recovery));
    println("coupling: " + std::to_string(coupling));
    println("reset_voltage: " + std::to_string(reset_voltage));
    println("reset_recovery: " + std::to_string(reset_recovery));
    println("dir_current: " + std::to_string(dir_current));
    println("voltage: " + std::to_string(voltage));
    println("recovery: " + std::to_string(recovery));
    print_array( "exc synapse: ", e_synapse);
    print_array( "inh synapse: ",  i_synapse);
    
  }

  //
  // spiking population
  //
  

  // constructor
  SpikingPopulation::SpikingPopulation(){
    int sz = 1;
    int bufsize = 10;
    std::string aname = "SpikingPopulation";
    NeuronType ntype = NeuronType::eRegular_spiking;

    units.reserve(sz);
    output.reserve(sz);
    data.reserve(sz);
    for(int i=0; i<sz; i++){
      units.push_back(new SpikingUnit(aname+"_"+std::to_string(i+1), ntype, 2));
      data.push_back(new Buffer(bufsize));
    }
    // default internal synapse is no conn
    internal_synapse = zeros(sz, sz);
    name = aname;
    

  }  

  SpikingPopulation::SpikingPopulation(std::string aname, int sz, NeuronType ntype, int bufsize){
    units.reserve(sz);
    output.reserve(sz);
    data.reserve(sz);
    for(int i=0; i<sz; i++){
      units.push_back(new SpikingUnit(aname+"_"+std::to_string(i+1), ntype, 2));
      data.push_back(new Buffer(bufsize));
    }
    // default internal synapse is no conn
    internal_synapse = zeros(sz, sz);
    name = aname;
  }

  SpikingPopulation::SpikingPopulation(std::string aname, int sz, NeuronType ntype, int bufsize, float noise){
    units.reserve(sz); //
    output.reserve(sz);
    data.reserve(sz);
    for(int i=0; i<sz; i++){
      units.push_back(new SpikingUnit(aname+"_"+std::to_string(i+1), ntype, 2, noise));
      data.push_back(new Buffer(bufsize));
    }
    // default internal synapse is no conn
    internal_synapse = zeros(sz, sz);
    name = aname;
    
  }

  SpikingPopulation::~SpikingPopulation(){
    for (int i = 0; i < units.size(); i++)
    {
        delete units[i];
        delete data[i];
    }
    
  }
  
  // accessors
  std::string SpikingPopulation::getName(){ return name;}
  int SpikingPopulation::getSize(){return units.size();}
  
  FloatList &SpikingPopulation::getOutput(){
    for(int i=0; i<units.size(); i++)
      output[i] = units[i]->getOutput();
    return output;
  }

  FloatList &SpikingPopulation::getNormOutput(){
    for(int i=0; i<units.size(); i++)
      output[i] = units[i]->getNormOutput();
    return output;
  }
  
  std::vector<Buffer*> &SpikingPopulation::getBuffers(){
    return data;  
  }
  
  void SpikingPopulation::reset(){
    for(int i=0; i<units.size(); i++){
      units[i]->reset();
    }
  }

  
  void SpikingPopulation::setInternalTopology(float** top, int sx, int sy){
    assert(sx <= units.size() && sy <= units.size());
    syn_x = sx;
    syn_y = sy;
    internal_synapse = top;  
  }
    
  
  // methods
  void SpikingPopulation::tick(){
    for(int i=0; i<units.size(); i++){
      units[i]->tick();
      data[i]->append(units[i]->getOutput());
    }
    updateConn(internal_synapse, units, syn_x, syn_y);   
  }
  
  void SpikingPopulation::setDirect(FloatList &val){
    for(int i=0; i<units.size(); i++)
      units[i]->setDirect(val[i]);
    
  }
  
  void SpikingPopulation::setResetVoltage(float val){
    // will increase, decrease all 
    for(int i=0; i<units.size(); i++)
      units[i]->setResetVoltage(val);
  }
  
  void SpikingPopulation::excite(FloatList &val, float** synapse, int sx, int sy){
    // will throw array out of bounds if not matching
    for(int j=0; j<sy; ++j){
      for(int i=0; i<sx; ++i){
          if(synapse[j][i] > 0)
            units[i]->excite(synapse[j][i] * val[j]);
      }
    }
    
  }
  
  void SpikingPopulation::inhibit(FloatList &val, float** synapse, int sx, int sy){
    /**
    val - array of unit outputs (any size)
    synapse - matrix of synapse mappings where 
      rows=sizeof(val), cols=sizeof(this population)
    */
    // will throw array out of bounds if not matching
    for(int j=0; j<sy; ++j){
      for(int i=0; i<sx; ++i){
          if(synapse[j][i] > 0)
            units[i]->inhibit(synapse[j][i] * val[j]);
          
      }
    }
    
  }
  
  void SpikingPopulation::modulateResetVoltage(float diff){
    // will increase, decrease all 
    for(int i=0; i<units.size(); i++)
      units[i]->modulateResetVoltage(diff);
  }
  
  void SpikingPopulation::modulateResetVoltage(FloatList &diff){
    // will increase, decrease per unit 
    for(int i=0; i<units.size(); i++)
      units[i]->modulateResetVoltage(diff[i]);
  }

  void SpikingPopulation::randomize(){
    for(int i=0; i<units.size(); i++)
      units[i]->randomize();
  }
  
  // privates
  void SpikingPopulation::updateConn(float** matrix, std::vector<SpikingUnit*> &units, int sx, int sy){

    for(int j=0; j<sy; ++j){
      for(int i=0; i<sx; ++i){
        if(matrix[j][i] > 0)
          units[i]->excite(units[j]->getOutput());//, units[j].getPhase());
        else if(matrix[j][i] < 0)
          units[i]->inhibit(units[j]->getOutput());//, units[j].getPhase());
      }
    }
  }
