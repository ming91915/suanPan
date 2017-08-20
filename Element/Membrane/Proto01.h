#pragma once
#include <Element/Element.h>

class Proto01 :
	public Element
{
	struct IntegrationPoint {
		vec coor;
		double weight, jacob_det;
		mat jacob, pn_pxy;
		mat P, A, B, BI;
		unique_ptr<Material> m_material;
	};

	static const unsigned m_node;
	static const unsigned m_dof;

	static mat mapping;

	double thickness;

	vector<unique_ptr<IntegrationPoint>> int_pt;

	mat ele_coor;

	vec trial_disp;   // displacement
	vec trial_alpha;  // strain
	vec trial_beta;   // stress

	vec current_disp;
	vec current_alpha;
	vec current_beta;
public:
	Proto01(const unsigned& , const uvec& , const unsigned& , const double&  = 1.);

	void initialize(const shared_ptr<Domain>&) override;

	int updateStatus() override;

	int commitStatus() override;
	int clearStatus() override;
	int resetStatus() override;
};

