#include <iomanip>
#include <iostream>
#include "CAveragine.h"
using namespace std;

CAveragine::CAveragine(char* fn, char* fn2){
	PT = new CPeriodicTable(fn2);
  atoms = new int[PT->size()];
  for(int i=0;i<PT->size();i++) atoms[i]=0;
  enrich = new vector<atomInfo>;
  loadTable(fn);
}

CAveragine::~CAveragine(){
  delete [] atoms;
  delete enrich;
}

void CAveragine::calcAveragine(double dMass, CHardklorVariant hv) {
  int i,j;
  int iso;
  int atom;
  double fixedD = dMass;
  double units;
  double *enr;
  double *abun;
  double maxAbun;
  double totAbun;
  enr = new double[PT->size()];
  for(i=0;i<PT->size();i++) enr[i]=0;

  for(i=0;i<hv.sizeEnrich();i++){
   
    atom = hv.atEnrich(i).atomNum;
    iso = enrich->at(atom).numIsotopes;
    abun = new double[iso];

    //Set our abundance array and find maximum
    maxAbun=0;
    for(j=0;j<iso;j++){
      abun[j]=enrich->at(atom).abundance->at(j);
      if(abun[j]>maxAbun) maxAbun=abun[j];
    }

    //Normalize to 1
    for(j=0;j<iso;j++) abun[j]/=maxAbun;
    
    //Normalize, then Calculate enrichment
    totAbun=0;
    enr[atom]=0;
    for(j=0;j<iso;j++){
      abun[j]/=maxAbun;
      if(j==hv.atEnrich(i).isotope) abun[j]=(1-hv.atEnrich(i).ape)*abun[j]+hv.atEnrich(i).ape;
      else abun[j]=(1-hv.atEnrich(i).ape)*abun[j];
      totAbun+=abun[j];
      enr[atom]+=abun[j]*enrich->at(atom).mass->at(j);
    }

    //Calculate average mass
    enr[atom] /= totAbun;
    enr[atom] -= PT->at(atom).mass;

    delete [] abun;
  }

  for(i=0;i<hv.sizeAtom();i++) {
    
    atoms[hv.atAtom(i).iLower]+=hv.atAtom(i).iUpper;
    fixedD -= hv.atAtom(i).iUpper*(PT->at(hv.atAtom(i).iLower).mass + enr[hv.atAtom(i).iLower]);
  }

  units = (fixedD)/(AVE_MASS + enr[1]*AVE_H + enr[6]*AVE_C + enr[7]*AVE_N + enr[8]*AVE_O + enr[16]*AVE_S);

  //Quick fix; assumes complete periodic table
  atoms[6] += (int)(AVE_C*units+0.5);
  atoms[8] += (int)(AVE_O*units+0.5);
  atoms[7] += (int)(AVE_N*units+0.5);
  atoms[16] += (int)(AVE_S*units+0.5);

  atoms[1] += (int)(fixedD-( ((int)(AVE_C*units+0.5)) * (PT->at(6).mass + enr[6]) + 
			     ((int)(AVE_N*units+0.5)) * (PT->at(7).mass + enr[7]) + 
			     ((int)(AVE_O*units+0.5)) * (PT->at(8).mass + enr[8]) +  
			     ((int)(AVE_S*units+0.5)) * (PT->at(16).mass + enr[16]) )+0.5);

  delete [] enr;

}

void CAveragine::defaultValues(){
  
  string el[109] = {"X","H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar",
    "K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr",
    "Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm",
    "Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po",
    "At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Hx","Cx","Nx",
    "Ox","Sx"};
  int sz[109] = {2,2,2,2,1,2,2,2,3,1,3,1,3,1,3,1,4,2,3,3,6,1,5,2,4,1,4,1,5,2,5,2,5,1,6,2,6,2,4,1,5,1,7,1,7,
    1,6,2,8,2,10,2,8,1,9,1,7,2,4,1,7,1,7,2,7,1,7,1,6,1,7,2,6,2,5,2,7,2,6,1,7,2,4,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,
    1,1,1,1,1,2,2,2,3,4};
  double m[322] = {1.000000000000,2.000000000000,1.007824600000,2.014102100000,3.016030000000,4.002600000000,
    6.015121000000,7.016003000000,9.012182000000,10.012937000000,11.009305000000,12.000000000000,13.003355400000,
    14.003073200000,15.000108800000,15.994914100000,16.999132200000,17.999161600000,18.998403200000,
    19.992435000000,20.993843000000,21.991383000000,22.989767000000,23.985042000000,24.985837000000,
    25.982593000000,26.981539000000,27.976927000000,28.976495000000,29.973770000000,30.973762000000,
    31.972070000000,32.971456000000,33.967866000000,35.967080000000,34.968853100000,36.965903400000,
    35.967545000000,37.962732000000,39.962384000000,38.963707000000,39.963999000000,40.961825000000,
    39.962591000000,41.958618000000,42.958766000000,43.955480000000,45.953689000000,47.952533000000,
    44.955910000000,45.952629000000,46.951764000000,47.947947000000,48.947871000000,49.944792000000,
    49.947161000000,50.943962000000,49.946046000000,51.940509000000,52.940651000000,53.938882000000,
    54.938047000000,53.939612000000,55.934939000000,56.935396000000,57.933277000000,58.933198000000,
    57.935346000000,59.930788000000,60.931058000000,61.928346000000,63.927968000000,62.939598000000,
    64.927793000000,63.929145000000,65.926034000000,66.927129000000,67.924846000000,69.925325000000,
    68.925580000000,70.924700000000,69.924250000000,71.922079000000,72.923463000000,73.921177000000,
    75.921401000000,74.921594000000,73.922475000000,75.919212000000,76.919912000000,77.919000000000,
    79.916520000000,81.916698000000,78.918336000000,80.916289000000,77.914000000000,79.916380000000,
    81.913482000000,82.914135000000,83.911507000000,85.910616000000,84.911794000000,86.909187000000,
    83.913430000000,85.909267000000,86.908884000000,87.905619000000,88.905849000000,89.904703000000,
    90.905644000000,91.905039000000,93.906314000000,95.908275000000,92.906377000000,91.906808000000,
    93.905085000000,94.905840000000,95.904678000000,96.906020000000,97.905406000000,99.907477000000,
    98.000000000000,95.907599000000,97.905287000000,98.905939000000,99.904219000000,100.905582000000,
    101.904348000000,103.905424000000,102.905500000000,101.905634000000,103.904029000000,104.905079000000,
    105.903478000000,107.903895000000,109.905167000000,106.905092000000,108.904757000000,105.906461000000,
    107.904176000000,109.903005000000,110.904182000000,111.902758000000,112.904400000000,113.903357000000,
    115.904754000000,112.904061000000,114.903880000000,111.904826000000,113.902784000000,114.903348000000,
    115.901747000000,116.902956000000,117.901609000000,118.903310000000,119.902200000000,121.903440000000,
    123.905274000000,120.903821000000,122.904216000000,119.904048000000,121.903054000000,122.904271000000,
    123.902823000000,124.904433000000,125.903314000000,127.904463000000,129.906229000000,126.904473000000,
    123.905894000000,125.904281000000,127.903531000000,128.904780000000,129.903509000000,130.905072000000,
    131.904144000000,133.905395000000,135.907214000000,132.905429000000,129.906282000000,131.905042000000,
    133.904486000000,134.905665000000,135.904553000000,136.905812000000,137.905232000000,137.907110000000,
    138.906347000000,135.907140000000,137.905985000000,139.905433000000,141.909241000000,140.907647000000,
    141.907719000000,142.909810000000,143.910083000000,144.912570000000,145.913113000000,147.916889000000,
    149.920887000000,145.000000000000,143.911998000000,146.914895000000,147.914820000000,148.917181000000,
    149.917273000000,151.919729000000,153.922206000000,150.919847000000,152.921225000000,151.919786000000,
    153.920861000000,154.922618000000,155.922118000000,156.923956000000,157.924099000000,159.927049000000,
    158.925342000000,155.925277000000,157.924403000000,159.925193000000,160.926930000000,161.926795000000,
    162.928728000000,163.929171000000,164.930319000000,161.928775000000,163.929198000000,165.930290000000,
    166.932046000000,167.932368000000,169.935461000000,168.934212000000,167.933894000000,169.934759000000,
    170.936323000000,171.936378000000,172.938208000000,173.938859000000,175.942564000000,174.940770000000,
    175.942679000000,173.940044000000,175.941406000000,176.943217000000,177.943696000000,178.945812000000,
    179.946545000000,179.947462000000,180.947992000000,179.946701000000,181.948202000000,182.950220000000,
    183.950928000000,185.954357000000,184.952951000000,186.955744000000,183.952488000000,185.953830000000,
    186.955741000000,187.955860000000,188.958137000000,189.958436000000,191.961467000000,190.960584000000,
    192.962917000000,189.959917000000,191.961019000000,193.962655000000,194.964766000000,195.964926000000,
    197.967869000000,196.966543000000,195.965807000000,197.966743000000,198.968254000000,199.968300000000,
    200.970277000000,201.970617000000,203.973467000000,202.972320000000,204.974401000000,203.973020000000,
    205.974440000000,206.975872000000,207.976627000000,208.980374000000,209.000000000000,210.000000000000,
    222.000000000000,223.000000000000,226.025000000000,227.028000000000,232.038054000000,231.035900000000,
    234.040946000000,235.043924000000,238.050784000000,237.048000000000,244.000000000000,243.000000000000,
    247.000000000000,247.000000000000,251.000000000000,252.000000000000,257.000000000000,258.000000000000,
    259.000000000000,260.000000000000,1.007824600000,2.014102100000,12.000000000000,13.003355400000,
    14.003073200000,15.000108800000,15.994914100000,16.999132200000,17.999161600000,31.972070000000,
    32.971456000000,33.967866000000,35.967080000000};
  double r[322] = {0.900000000000,0.100000000000,0.999855000000,0.000145000000,0.000001380000,0.999998620000,
    0.075000000000,0.925000000000,1.000000000000,0.199000000000,0.801000000000,0.989160000000,0.010840000000,
    0.996330000000,0.003660000000,0.997576009706,0.000378998479,0.002044991815,1.000000000000,0.904800000000,
    0.002700000000,0.092500000000,1.000000000000,0.789900000000,0.100000000000,0.110100000000,1.000000000000,
    0.922300000000,0.046700000000,0.031000000000,1.000000000000,0.950210000000,0.007450000000,0.042210000000,
    0.000130000000,0.755290000000,0.244710000000,0.003370000000,0.000630000000,0.996000000000,0.932581000000,
    0.000117000000,0.067302000000,0.969410000000,0.006470000000,0.001350000000,0.020860000000,0.000040000000,
    0.001870000000,1.000000000000,0.080000000000,0.073000000000,0.738000000000,0.055000000000,0.054000000000,
    0.002500000000,0.997500000000,0.043450000000,0.837900000000,0.095000000000,0.023650000000,1.000000000000,
    0.059000000000,0.917200000000,0.021000000000,0.002800000000,1.000000000000,0.682700000000,0.261000000000,
    0.011300000000,0.035900000000,0.009100000000,0.691700000000,0.308300000000,0.486000000000,0.279000000000,
    0.041000000000,0.188000000000,0.006000000000,0.601080000000,0.398920000000,0.205000000000,0.274000000000,
    0.078000000000,0.365000000000,0.078000000000,1.000000000000,0.009000000000,0.091000000000,0.076000000000,
    0.236000000000,0.499000000000,0.089000000000,0.506900000000,0.493100000000,0.003500000000,0.022500000000,
    0.116000000000,0.115000000000,0.570000000000,0.173000000000,0.721700000000,0.278300000000,0.005600000000,
    0.098600000000,0.070000000000,0.825800000000,1.000000000000,0.514500000000,0.112200000000,0.171500000000,
    0.173800000000,0.028000000000,1.000000000000,0.148400000000,0.092500000000,0.159200000000,0.166800000000,
    0.095500000000,0.241300000000,0.096300000000,1.000000000000,0.055400000000,0.018600000000,0.127000000000,
    0.126000000000,0.171000000000,0.316000000000,0.186000000000,1.000000000000,0.010200000000,0.111400000000,
    0.223300000000,0.273300000000,0.264600000000,0.117200000000,0.518390000000,0.481610000000,0.012500000000,
    0.008900000000,0.124900000000,0.128000000000,0.241300000000,0.122200000000,0.287300000000,0.074900000000,
    0.043000000000,0.957000000000,0.009700000000,0.006500000000,0.003600000000,0.145300000000,0.076800000000,
    0.242200000000,0.085800000000,0.325900000000,0.046300000000,0.057900000000,0.574000000000,0.426000000000,
    0.000950000000,0.025900000000,0.009050000000,0.047900000000,0.071200000000,0.189300000000,0.317000000000,
    0.338700000000,1.000000000000,0.001000000000,0.000900000000,0.019100000000,0.264000000000,0.041000000000,
    0.212000000000,0.269000000000,0.104000000000,0.089000000000,1.000000000000,0.001060000000,0.001010000000,
    0.024200000000,0.065930000000,0.078500000000,0.112300000000,0.717000000000,0.000900000000,0.999100000000,
    0.001900000000,0.002500000000,0.884300000000,0.111300000000,1.000000000000,0.271300000000,0.121800000000,
    0.238000000000,0.083000000000,0.171900000000,0.057600000000,0.056400000000,1.000000000000,0.031000000000,
    0.150000000000,0.113000000000,0.138000000000,0.074000000000,0.267000000000,0.227000000000,0.478000000000,
    0.522000000000,0.002000000000,0.021800000000,0.148000000000,0.204700000000,0.156500000000,0.248400000000,
    0.218600000000,1.000000000000,0.000600000000,0.001000000000,0.023400000000,0.189000000000,0.255000000000,
    0.249000000000,0.282000000000,1.000000000000,0.001400000000,0.016100000000,0.336000000000,0.229500000000,
    0.268000000000,0.149000000000,1.000000000000,0.001300000000,0.030500000000,0.143000000000,0.219000000000,
    0.161200000000,0.318000000000,0.127000000000,0.974100000000,0.025900000000,0.001620000000,0.052060000000,
    0.186060000000,0.272970000000,0.136290000000,0.351000000000,0.000120000000,0.999880000000,0.001200000000,
    0.263000000000,0.142800000000,0.307000000000,0.286000000000,0.374000000000,0.626000000000,0.000200000000,
    0.015800000000,0.016000000000,0.133000000000,0.161000000000,0.264000000000,0.410000000000,0.373000000000,
    0.627000000000,0.000100000000,0.007900000000,0.329000000000,0.338000000000,0.253000000000,0.072000000000,
    1.000000000000,0.001500000000,0.100000000000,0.169000000000,0.231000000000,0.132000000000,0.298000000000,
    0.068500000000,0.295240000000,0.704760000000,0.014000000000,0.241000000000,0.221000000000,0.524000000000,
    1.000000000000,1.000000000000,1.000000000000,1.000000000000,1.000000000000,1.000000000000,1.000000000000,
    1.000000000000,1.000000000000,0.000055000000,0.007200000000,0.992745000000,1.000000000000,1.000000000000,
    1.000000000000,1.000000000000,1.000000000000,1.000000000000,1.000000000000,1.000000000000,1.000000000000,
    1.000000000000,1.000000000000,0.999855000000,0.000145000000,0.989160000000,0.010840000000,0.996330000000,
    0.003660000000,0.997576009706,0.000378998479,0.002044991815,0.950210000000,0.007450000000,0.042210000000,
    0.000130000000};
  
  int i=0;
  int j=0;
  int k;
  atomInfo a;

  for(k=0;k<109;k++){    
    strcpy(a.symbol,&el[k][0]);
    a.numIsotopes=sz[k];
    a.mass->clear();
    a.abundance->clear();
    for(j=0;j<a.numIsotopes;j++){
      a.mass->push_back(m[i]);
			a.abundance->push_back(r[i]);
      i++;
    }
    enrich->push_back(a);
  }

}

void CAveragine::clear(){
  delete [] atoms;
  atoms = new int[PT->size()];
  for(int i=0;i<PT->size();i++) atoms[i]=0;
}

int CAveragine::getElement(int i){
  return atoms[i];
}

void CAveragine::getAveragine(char *str){
  char tstr[10];
  int i;
  str[0]=0;

  //Create formula string
  for(i=0;i<PT->size();i++){
    if(atoms[i]>0){
      sprintf(tstr,"%s%i",PT->at(i).symbol,atoms[i]);
      strcat(str,tstr);
    }
  }

}

double CAveragine::getMonoMass(){
  double d=0;
  int i;

  for(i=0;i<PT->size();i++) d+=atoms[i]*PT->at(i).mass;
  return d;

}

CPeriodicTable* CAveragine::getPT(){
  return PT;
}

void CAveragine::loadTable(char* c){

  FILE *f;
  int  j;
  atomInfo a;
	double d1,d2;

  if(c==NULL || strlen(c)==0){
    defaultValues();
    return;
  }

  f = fopen(c,"rt");
  if(f==NULL) {
    cout << "Cannot read " << c << endl;
    exit(-1);
  }

  while(!feof(f)){
   
		fscanf(f,"%2s\t%d\n",&a.symbol,&a.numIsotopes);
    a.mass->clear();
    a.abundance->clear();

    for(j=0;j<a.numIsotopes;j++){
      fscanf(f,"%lf\t%lf\n",&d1,&d2);
			a.mass->push_back(d1);
			a.abundance->push_back(d2);
    }

    enrich->push_back(a);
		fscanf(f," \n");

  }

  fclose(f);

}

//This is not to be used normally - just here to make my life easier.
void CAveragine::setAveragine(int C, int H, int N, int O, int S){
  atoms[1]=H;
  atoms[6]=C;
  atoms[7]=N;
  atoms[8]=O;
  atoms[16]=S;
}
