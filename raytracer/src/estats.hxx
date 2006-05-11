#ifndef _ESTATS_RAY_TRACER_HXX_
#define _ESTATS_RAY_TRACER_HXX_

class Estats
{
protected:
  static Estats inst;
  Estats(){}
public:
  Estats& getInstance(){return inst;}
};

