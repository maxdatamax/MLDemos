/*********************************************************************
MLDemos: A User-Friendly visualization toolkit for machine learning
Copyright (C) 2010  Basilio Noris
Contact: mldemos@b4silio.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License,
version 3 as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free
Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*********************************************************************/
#ifndef _BASICMATH_H_
#define _BASICMATH_H_

#include "public.h"
#include <vector>
#include <float.h>
#include <math.h>

#define PIf	3.1415926535897932384626433832795f

/*!
 * Cubic interpolation class adapted from an algorithm by Marino 'Mutilate' Alge
 * works with cvVec2 and Vec3 types
 */
template <class T>
class CCubic
{
private:
	T a, b, c, d;

public:
	/*!
		Constructor, takes 4 points as input for the interpolation
		\param x0-x3 the input points for interpolation
	*/
	CCubic(T x0, T x1, T x2, T x3)
	{
//		a = x1;
//		b = (x2 - x0)*0.5f;
//		c = x0 - x1*2.5f + x2*2.0f - x3*0.5f;
//		d = (x3 - x0)*0.5f - (x2 - x1)*1.5f;
		a = x1;
		b = MulS(Sub(x2,x0),0.5f);
		c = Add(Sub(x0,MulS(x1,2.5f)),Sub(MulS(x2,2.0f),MulS(x3,0.5f)));
		d = Sub(MulS(Sub(x3,x0),0.5f),MulS(Sub(x2,x1),1.5f));
	}

	/*!
		The interpolation prediction function
		\param t the input time
		\return returns the value expected at time t
	*/
	T X(float t) const
	{
		//return a + (b + (c + d*t)*t)*t;
		return Add(a,MulS(Add(b,MulS(Add(c,MulS(d,t)),t)),t));
	}

	/*!
		All in one interpolation function
		\param x0-x3 the input points for interpolation
		\param t the input time
		\return returns the value expected at time t, using points x0-x3 for interpolation
	*/
	static T X(T x0, T x1, T x2, T x3, float t)
	{
		//return x1 + ((x2 - x0)*0.5f + (x0 - x1*2.5f + x2*2.0f - x3*0.5f + (x3 - x0)*0.5f - (x2 - x1)*1.5f*t)*t)*t;
		T asd7 = Sub(x0, MulS(x1,2.5f));
		T asd6 = Sub(MulS(x2,2.0f), MulS(x3,0.5f));
		T asd5 = Sub(MulS(Sub(x3,x0),0.5f), MulS(Sub(x2,x1),1.5f*t));
		T asd4 = Add(asd5,Add(asd6,asd7));
		T asd3 = MulS(asd4,t);
		T asd2 = Add(MulS(Sub(x2,x0),0.5f), asd3);
		T asd  = MulS(asd2,t);
		return Add(x1, asd);
	}
};

/*!
	Random Permutations array
	\param length the length of the output array
	\param seed random seed (optional) clock used otherwise
	\return returns an array of indices between 0 and 'length' randomly sorted
*/
static u32 *randPerm(u32 length, s32 seed=-1)
{
        u32 i=0;
	u32 *perm = (u32 *) malloc(sizeof(u32) * length);
	u32 *usable = (u32 *) malloc(sizeof(u32) * length);
	int uLength = length;
        if(seed != -1) srand(seed);

        for(int i=0; i<length; i++)
	{
		perm[i] = 0;
		usable[i] = i;
	}

	for (int i=0; i<length; i++)
	{
		int r = 0;
		if(RAND_MAX <= 0x7fff)
			r = ((rand()<< 7) + rand()) % uLength;
		else
			r = rand() % uLength;

		perm[i] = usable[r];
		uLength--;
		usable[r] = usable[uLength];
		usable[uLength] = 0;
		if(!uLength) break;
	}
	free(usable);

	return perm;
}


// matlab code to generate the table
// erf(x) = (x>0?1:-1) * erftable((int)(min(6.f,abs(x))*100));
// 0:0.01:6
static const float erftable [] = 
{
	0.0000000000000000f, 0.0112834155558496f, 0.0225645746918449f, 0.0338412223417354f, 0.0451111061451247f, 0.0563719777970166f, 0.0676215943933084f, 0.0788577197708907f, 0.0900781258410182f, 0.1012805939146269f, 0.1124629160182849f, 0.1236228961994743f, 0.1347583518199201f, 0.1458671148356958f, 0.1569470330628558f, 0.1679959714273635f, 0.1790118131981057f, 0.1899924612018088f, 0.2009358390186958f, 0.2118398921577497f, 0.2227025892104785f, 0.2335219229821036f, 0.2442959115991287f, 0.2550225995922731f, 0.2657000589537920f, 0.2763263901682369f, 0.2868997232157491f, 0.2974182185470128f, 0.3078800680290340f, 0.3182834958609522f, 0.3286267594591273f, 0.3389081503107902f, 0.3491259947955827f, 0.3592786549743590f, 0.3693645293446587f, 0.3793820535623103f, 0.3893297011286642f, 0.3992059840429992f, 0.4090094534196940f, 0.4187387000697961f, 0.4283923550466685f, 0.4379690901554394f, 0.4474676184260253f, 0.4568866945495403f, 0.4662251152779575f, 0.4754817197869237f, 0.4846553900016797f, 0.4937450508860821f, 0.5027496706947650f, 0.5116682611885233f, 0.5204998778130465f, 0.5292436198411704f, 0.5378986304788544f, 0.5464640969351416f, 0.5549392504563904f, 0.5633233663251089f, 0.5716157638237684f, 0.5798158061639961f, 0.5879229003816007f, 0.5959364971979084f, 0.6038560908479259f, 0.6116812188758802f, 0.6194114618987212f, 0.6270464433381957f, 0.6345858291221413f, 0.6420293273556719f, 0.6493766879629542f, 0.6566277023003051f, 0.6637822027413580f, 0.6708400622350779f, 0.6778011938374186f, 0.6846655502174442f, 0.6914331231387512f, 0.6981039429170445f, 0.7046780778547458f, 0.7111556336535152f, 0.7175367528055909f, 0.7238216139648592f, 0.7300104312985789f, 0.7361034538206912f, 0.7421009647076605f, 0.7480032805977895f, 0.7538107508749625f, 0.7595237569377731f, 0.7651427114549946f, 0.7706680576083524f, 0.7761002683235567f, 0.7814398454905507f, 0.7866873191739325f, 0.7918432468144954f, 0.7969082124228322f, 0.8018828257659413f, 0.8067677215477618f, 0.8115635585845578f, 0.8162710189760625f, 0.8208908072732779f, 0.8254236496438183f, 0.8298702930356671f, 0.8342315043402079f, 0.8385080695553697f, 0.8427007929497148f, 0.8468104962282766f, 0.8508380177009420f, 0.8547842114541484f, 0.8586499465266515f, 0.8624361060900967f, 0.8661435866351080f, 0.8697732971635868f, 0.8733261583878896f, 0.8768031019375383f, 0.8802050695740817f, 0.8835330124147180f, 0.8867878901652547f, 0.8899706703629624f, 0.8930823276298567f, 0.8961238429369151f, 0.8990962028797120f, 0.9020003989659357f, 0.9048374269152169f, 0.9076082859716850f, 0.9103139782296355f, 0.9129555079726694f, 0.9155338810266469f, 0.9180501041267614f, 0.9205051842990297f, 0.9229001282564582f, 0.9252359418101295f, 0.9275136292954247f, 0.9297341930135782f, 0.9318986326887336f, 0.9340079449406524f, 0.9360631227731995f, 0.9380651550787114f, 0.9400150261583302f, 0.9419137152583653f, 0.9437621961227241f, 0.9455614365614331f, 0.9473123980352520f, 0.9490160352563626f, 0.9506732958050965f, 0.9522851197626489f, 0.9538524393597054f, 0.9553761786408961f, 0.9568572531449688f, 0.9582965696005648f, 0.9596950256374592f, 0.9610535095131181f, 0.9623728998544057f, 0.9636540654142689f, 0.9648978648432043f, 0.9661051464753108f, 0.9672767481287117f, 0.9684134969201232f, 0.9695162090933357f, 0.9705856898613637f, 0.9716227332620125f, 0.9726281220266002f, 0.9736026274615670f, 0.9745470093426969f, 0.9754620158216676f, 0.9763483833446440f, 0.9772068365826185f, 0.9780380883732035f, 0.9788428396735702f, 0.9796217795242320f, 0.9803755850233603f, 0.9811049213113221f, 0.9818104415651265f, 0.9824927870024649f, 0.9831525868950262f, 0.9837904585907746f, 0.9844070075448683f, 0.9850028273589058f, 0.9855784998281805f, 0.9861345949966329f, 0.9866716712191824f, 0.9871902752311301f, 0.9876909422243223f, 0.9881741959297683f, 0.9886405487064082f, 0.9890905016357308f, 0.9895245446219444f, 0.9899431564974077f, 0.9903468051330306f, 0.9907359475533626f, 0.9911110300560857f, 0.9914724883356396f, 0.9918207476107068f, 0.9921562227552937f, 0.9924793184331480f, 0.9927904292352574f, 0.9930899398201836f, 0.9933782250569847f, 0.9936556501704964f, 0.9939225708887325f, 0.9941793335921891f, 0.9944262754648279f, 0.9946637246465300f, 0.9948920003868136f, 0.9951114131996171f, 0.9953222650189527f, 0.9955248493552482f, 0.9957194514521921f, 0.9959063484439121f, 0.9960858095123195f, 0.9962580960444569f, 0.9964234617896959f, 0.9965821530166383f, 0.9967344086695764f, 0.9968804605243777f, 0.9970205333436670f, 0.9971548450311778f, 0.9972836067851606f, 0.9974070232507333f, 0.9975252926710697f, 0.9976386070373253f, 0.9977471522372077f, 0.9978511082021002f, 0.9979506490526588f, 0.9980459432428015f, 0.9981371537020181f, 0.9982244379759344f, 0.9983079483650648f, 0.9983878320616981f, 0.9984642312848625f, 0.9985372834133188f, 0.9986071211165417f, 0.9986738724836455f, 0.9987376611502190f, 0.9987986064230412f, 0.9988568234026434f, 0.9989124231037001f, 0.9989655125732240f, 0.9990161950065498f, 0.9990645698610920f, 0.9991107329678676f, 0.9991547766407751f, 0.9991967897836264f, 0.9992368579949287f, 0.9992750636704192f, 0.9993114861033550f, 0.9993462015825647f, 0.9993792834882711f, 0.9994108023856942f, 0.9994408261164486f, 0.9994694198877490f, 0.9994966463594419f, 0.9995225657288811f, 0.9995472358136659f, 0.9995707121322661f, 0.9995930479825550f, 0.9996142945182758f, 0.9996345008234653f, 0.9996537139848649f, 0.9996719791623431f, 0.9996893396573607f, 0.9997058369795080f, 0.9997215109111428f, 0.9997363995701628f, 0.9997505394709432f, 0.9997639655834707f, 0.9997767113907082f, 0.9997888089442237f, 0.9998002889181156f, 0.9998111806612684f, 0.9998215122479760f, 0.9998313105269614f, 0.9998406011688324f, 0.9998494087120056f, 0.9998577566071316f, 0.9998656672600594f, 0.9998731620733716f, 0.9998802614865254f, 0.9998869850146334f, 0.9998933512859194f, 0.9998993780778804f, 0.9999050823521898f, 0.9999104802883753f, 0.9999155873163016f, 0.9999204181474947f, 0.9999249868053346f, 0.9999293066541523f, 0.9999333904272598f, 0.9999372502539452f, 0.9999408976854610f, 0.9999443437200386f, 0.9999475988269556f, 0.9999506729696857f, 0.9999535756281590f, 0.9999563158201617f, 0.9999589021219005f, 0.9999613426877595f, 0.9999636452692755f, 0.9999658172333573f, 0.9999678655797740f, 0.9999697969579359f, 0.9999716176829931f, 0.9999733337512747f, 0.9999749508550908f, 0.9999764743969193f, 0.9999779095030015f, 0.9999792610363629f, 0.9999805336092855f, 0.9999817315952467f, 0.9999828591403461f, 0.9999839201742398f, 0.9999849184206001f, 0.9999858574071167f, 0.9999867404750594f, 0.9999875707884177f, 0.9999883513426329f, 0.9999890849729398f, 0.9999897743623336f, 0.9999904220491747f, 0.9999910304344468f, 0.9999916017886847f, 0.9999921382585810f, 0.9999926418732865f, 0.9999931145504183f, 0.9999935581017863f, 0.9999939742388482f, 0.9999943645779092f, 0.9999947306450711f, 0.9999950738809456f, 0.9999953956451422f, 0.9999956972205364f, 0.9999959798173321f, 0.9999962445769250f, 0.9999964925755764f, 0.9999967248279045f, 0.9999969422902035f, 0.9999971458635975f, 0.9999973363970345f, 0.9999975146901312f, 0.9999976814958739f, 0.9999978375231799f, 0.9999979834393308f, 0.9999981198722784f, 0.9999982474128331f, 0.9999983666167385f, 0.9999984780066371f, 0.9999985820739346f, 0.9999986792805644f, 0.9999987700606605f, 0.9999988548221410f, 0.9999989339482065f, 0.9999990077987595f, 0.9999990767117464f, 0.9999991410044279f, 0.9999992009745795f, 0.9999992569016276f, 0.9999993090477226f, 0.9999993576587528f, 0.9999994029653040f, 0.9999994451835634f, 0.9999994845161754f, 0.9999995211530479f, 0.9999995552721144f, 0.9999995870400529f, 0.9999996166129631f, 0.9999996441370069f, 0.9999996697490110f, 0.9999996935770344f, 0.9999997157409060f, 0.9999997363527273f, 0.9999997555173494f, 0.9999997733328196f, 0.9999997898908039f, 0.9999998052769828f, 0.9999998195714259f, 0.9999998328489421f, 0.9999998451794108f, 0.9999998566280922f, 0.9999998672559198f, 0.9999998771197746f, 0.9999998862727435f, 0.9999998947643614f, 0.9999999026408388f, 0.9999999099452765f, 0.9999999167178646f, 0.9999999229960725f, 0.9999999288148247f, 0.9999999342066670f, 0.9999999392019217f, 0.9999999438288334f, 0.9999999481137065f, 0.9999999520810322f, 0.9999999557536089f, 0.9999999591526549f, 0.9999999622979134f, 0.9999999652077514f, 0.9999999678992515f, 0.9999999703882987f, 0.9999999726896611f, 0.9999999748170654f, 0.9999999767832677f, 0.9999999786001196f, 0.9999999802786297f, 0.9999999818290218f, 0.9999999832607887f, 0.9999999845827421f, 0.9999999858030606f, 0.9999999869293328f, 0.9999999879685986f, 0.9999999889273877f, 0.9999999898117551f, 0.9999999906273142f, 0.9999999913792682f, 0.9999999920724392f, 0.9999999927112944f, 0.9999999932999724f, 0.9999999938423057f, 0.9999999943418427f, 0.9999999948018690f, 0.9999999952254246f, 0.9999999956153229f, 0.9999999959741669f, 0.9999999963043638f, 0.9999999966081397f, 0.9999999968875528f, 0.9999999971445058f, 0.9999999973807567f, 0.9999999975979301f, 0.9999999977975265f, 0.9999999979809319f, 0.9999999981494259f, 0.9999999983041898f, 0.9999999984463144f, 0.9999999985768053f, 0.9999999986965913f, 0.9999999988065282f, 0.9999999989074059f, 0.9999999989999523f, 0.9999999990848385f, 0.9999999991626829f, 0.9999999992340556f, 0.9999999992994814f, 0.9999999993594437f, 0.9999999994143880f, 0.9999999994647240f, 0.9999999995108290f, 0.9999999995530502f, 0.9999999995917070f, 0.9999999996270934f, 0.9999999996594795f, 0.9999999996891137f, 0.9999999997162244f, 0.9999999997410216f, 0.9999999997636982f, 0.9999999997844314f, 0.9999999998033839f, 0.9999999998207052f, 0.9999999998365327f, 0.9999999998509920f, 0.9999999998641989f, 0.9999999998762595f, 0.9999999998872711f, 0.9999999998973228f, 0.9999999999064966f, 0.9999999999148674f, 0.9999999999225040f, 0.9999999999294694f, 0.9999999999358213f, 0.9999999999416126f, 0.9999999999468917f, 0.9999999999517030f, 0.9999999999560869f, 0.9999999999600808f, 0.9999999999637186f, 0.9999999999670313f, 0.9999999999700474f, 0.9999999999727929f, 0.9999999999752915f, 0.9999999999775653f, 0.9999999999796336f, 0.9999999999815150f, 0.9999999999832258f, 0.9999999999847813f, 0.9999999999861953f, 0.9999999999874802f, 0.9999999999886479f, 0.9999999999897087f, 0.9999999999906721f, 0.9999999999915470f, 0.9999999999923415f, 0.9999999999930624f, 0.9999999999937168f, 0.9999999999943107f, 0.9999999999948495f, 0.9999999999953380f, 0.9999999999957810f, 0.9999999999961828f, 0.9999999999965470f, 0.9999999999968769f, 0.9999999999971760f, 0.9999999999974469f, 0.9999999999976923f, 0.9999999999979145f, 0.9999999999981156f, 0.9999999999982978f, 0.9999999999984626f, 0.9999999999986117f, 0.9999999999987466f, 0.9999999999988686f, 0.9999999999989789f, 0.9999999999990787f, 0.9999999999991689f, 0.9999999999992504f, 0.9999999999993240f, 0.9999999999993905f, 0.9999999999994507f, 0.9999999999995048f, 0.9999999999995539f, 0.9999999999995981f, 0.9999999999996381f, 0.9999999999996740f, 0.9999999999997065f, 0.9999999999997358f, 0.9999999999997622f, 0.9999999999997861f, 0.9999999999998075f, 0.9999999999998268f, 0.9999999999998444f, 0.9999999999998600f, 0.9999999999998741f, 0.9999999999998870f, 0.9999999999998983f, 0.9999999999999087f, 0.9999999999999181f, 0.9999999999999263f, 0.9999999999999338f, 0.9999999999999407f, 0.9999999999999467f, 0.9999999999999523f, 0.9999999999999572f, 0.9999999999999616f, 0.9999999999999655f, 0.9999999999999691f, 0.9999999999999722f, 0.9999999999999751f, 0.9999999999999778f, 0.9999999999999800f, 0.9999999999999821f, 0.9999999999999840f, 0.9999999999999857f, 0.9999999999999871f, 0.9999999999999885f, 0.9999999999999898f, 0.9999999999999908f, 0.9999999999999918f, 0.9999999999999927f, 0.9999999999999933f, 0.9999999999999941f, 0.9999999999999947f, 0.9999999999999953f, 0.9999999999999958f, 0.9999999999999962f, 0.9999999999999967f, 0.9999999999999970f, 0.9999999999999973f, 0.9999999999999976f, 0.9999999999999979f, 0.9999999999999981f, 0.9999999999999983f, 0.9999999999999985f, 0.9999999999999987f, 0.9999999999999988f, 0.9999999999999989f, 0.9999999999999991f, 0.9999999999999991f, 0.9999999999999992f, 0.9999999999999993f, 0.9999999999999993f, 0.9999999999999994f, 0.9999999999999996f, 0.9999999999999996f, 0.9999999999999996f, 0.9999999999999997f, 0.9999999999999998f, 0.9999999999999998f, 0.9999999999999998f, 0.9999999999999998f, 0.9999999999999998f, 0.9999999999999998f, 0.9999999999999998f, 0.9999999999999999f, 0.9999999999999999f, 0.9999999999999999f, 0.9999999999999999f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f, 1.0000000000000000f
};
/*!
	Gaussian Error Function
*/
static float erf(const float x)
{
	if(x>0) return erftable[(int)(min(6.f,x)*100)];
	return -erftable[(int)(min(6.f,-x)*100)];
}

enum distEnum {DIST_EUCLIDEAN, DIST_MANHATTAN, DIST_INFINITE} ;

inline float Distance(float *a, float *b, u32 dim, distEnum metric)
{
	float dist = 0;
	switch(metric)
	{
	case DIST_EUCLIDEAN:
		FOR(i, dim) dist += powf(a[i]-b[i],2);
		dist = sqrtf(dist);
		break;
	case DIST_MANHATTAN:
		FOR(i, dim) dist += (float)abs((double)a[i]-(double)b[i]);
		break;
	case DIST_INFINITE:
		FOR(i, dim) dist = max(dist, (float)abs((double)a[i]-(double)b[i]));
		break;
	}
	return dist;
}

/**
  * Performs soft-kmeans clustering. The number of clusters is inferred from the dimension of means
  * it is advisable to normalize the data in order to avoid numerical problems due to the negative exp function
  *	\param points the input points
  * \param means, the starting cluster positions
  * \param dim, the dimension of the point data
  * \param beta, the stiffness of the soft-kmeans boundary
  * \return returns the weights for each points for each cluster
  *

*/
inline std::vector<fvec> SoftKMeans(const std::vector<float *> points, std::vector<float *> *means, unsigned int dim, float beta)
{
	unsigned int clusters = means->size();

	std::vector<fvec> weights;
	for (unsigned int i=0; i < points.size(); i++)
	{
		weights.push_back(fvec());
		for (unsigned int j=0; j < clusters; j++) weights[i].push_back(0);
	}

	fvec distances;
	for (unsigned int i=0; i < clusters; i++) distances.push_back(0);


	// Expectation Step
	for (unsigned int i=0; i < points.size(); i++)
	{
		float distanceSum = 0;
		for (unsigned int j=0; j < clusters; j++)
		{
			distances[j] = expf(-beta*Distance(points[i], (*means)[j], dim, DIST_EUCLIDEAN));
			distanceSum += distances[j];
		}

		for (unsigned int j=0; j < clusters; j++)
		{
			weights[i][j] = distances[j] / distanceSum;
		}
	}

	// Maximization Step
	fvec perCluster;

	for (unsigned int i=0; i < clusters; i++)
	{
		for (unsigned int j=0; j < dim; j++) (*means)[i][j] = 0;
		perCluster.push_back(0);
	}

	for (unsigned int i=0; i < points.size(); i++)
	{
		for (unsigned int j=0; j < clusters; j++)
		{
			for (unsigned int k=0; k < dim; k++)
			{
				(*means)[j][k] += points[i][k] * weights[i][j];
			}
			perCluster[j] += weights[i][j];
		}
	}

	for (unsigned int i=0; i < clusters; i++)
	{
		if(perCluster[i] != 0)
		{
			for (unsigned int j=0; j < dim; j++)
			{
				(*means)[i][j] /= perCluster[i];
			}
		}
	}

	return weights;
}

static std::vector<float> Quartiles(std::vector<float> srcdata)
{
	std::vector<float> quartiles;
	if(!srcdata.size())
	{
		quartiles.resize(5,0);
		return quartiles;
	}
	// we take out all non-zero elements
	std::vector<float> data;
	FOR(i, srcdata.size()) if(srcdata[i] != 0) data.push_back(srcdata[i]);
	if(!data.size())
	{
		quartiles.resize(5,0);
		return quartiles;
	}
	if(data.size()==1)
	{
		quartiles.resize(5,data[0]);
		return quartiles;
	}
	float mean = 0;
	float sigma = 0;
	FOR(i, data.size()) mean += data[i] / data.size();
	FOR(i, data.size()) sigma += powf(data[i]-mean,2);
	sigma = sqrtf(sigma/data.size());

	std::vector<float> outliers;
	std::vector<float> sorted;
	if(sigma==0)
	{
		sorted = data;
	}
	else
	{
		// we look for outliers using the 3*sigma rule
		FOR(i, data.size())
		{
			if (data[i] - mean < 3*sigma)
				sorted.push_back(data[i]);
			else outliers.push_back(data[i]);
		}
	}
	if(!sorted.size())
	{
		quartiles.resize(5,0);
		return quartiles;
	}
	sort(sorted.begin(), sorted.end());
	int count = sorted.size();
	int half = count/2;
	float bottom = sorted[0];
	float top = sorted[sorted.size()-1];

	float median = count%2 ? sorted[half] : (sorted[half] + sorted[half - 1])/2;
	float quartLow, quartHi;
	if(count < 4)
	{
		quartLow = bottom;
		quartHi = top;
	}
	else
	{
		quartLow = half%2 ? sorted[half/2] : (sorted[half/2] + sorted[half/2 - 1])/2;
		quartHi = half%2 ? sorted[half*3/2] : (sorted[half*3/2] + sorted[half*3/2 - 1])/2;
	}
	quartiles.push_back(bottom);
	quartiles.push_back(quartLow);
	quartiles.push_back(median);
	quartiles.push_back(quartHi);
	quartiles.push_back(top);
	return quartiles;
}

static std::vector<float> MeanStd(std::vector<float> srcdata)
{
	std::vector<float> results;
	results.resize(2,0);
	if(!srcdata.size())
	{
		return results;
	}
	if(srcdata.size() == 1)
	{
		results[0] = srcdata[0];
		results[1] = 0;
		return results;
	}
	float mean = 0;
	float stdev = 0;
	FOR(i, srcdata.size())
	{
		mean += srcdata[i];
	}
	mean /= srcdata.size();
	FOR(i, srcdata.size())
	{
		stdev += (srcdata[i]-mean)*(srcdata[i]-mean);
	}
	stdev = sqrtf(stdev);
	results[0] = mean;
	results[1] = stdev;
	return results;
}


#endif // _BASICMATH_H_
