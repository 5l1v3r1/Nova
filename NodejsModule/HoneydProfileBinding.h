#ifndef HONEYDPROFILEBINDING_H
#define HONEYDPROFILEBINDING_H

#include <node.h>
#include <v8.h>
#include "v8Helper.h"
#include "HoneydConfiguration/Profile.h"

class HoneydProfileBinding : public node::ObjectWrap
{

public:
	static void Init(v8::Handle<v8::Object> target);
  
	Nova::Profile *GetChild();

	bool SetName(std::string name);
	bool SetPersonality(std::string personality);
	bool SetUptimeMin(uint uptimeMin);
	bool SetUptimeMax(uint uptimeMax);
	bool SetDropRate(std::string dropRate);
	bool SetParentProfile(std::string parentName);
	bool SetCount(int count);
	bool SetIsGenerated(bool isGenerated);
	bool AddPortSet(std::string portSetName);
	bool ClearPorts();

private:
	//The parent name is needed to know where to put the profile in the tree,
	//The profile name is the name of the profile to edit, or add
	HoneydProfileBinding(std::string parentName, std::string profileName);
	~HoneydProfileBinding();

	//Odd ball out, because it needs 5 parameters. More than InvoleWrappedMethod can handle
	static v8::Handle<v8::Value> AddPort(const v8::Arguments& args);
	static v8::Handle<v8::Value> SetVendors(const v8::Arguments& args);

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	Nova::Profile *m_profile;
	bool isNewProfile;
};

#endif
