/*
  @copyright Steve Keen 2018
  @author Russell Standish
  This file is part of Minsky.

  Minsky is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Minsky is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Minsky.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xvector.h"
#include "tensorOp.h"
#include "tensorVal.h"
using namespace civita;

#include <UnitTest++/UnitTest++.h>

#include <exception>
using namespace std;

#include <boost/date_time.hpp>
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;

SUITE(TensorOps)
{
  TEST(tensorValVectorIndex)
  {
    TensorVal tv(vector<unsigned>{5,3,2});
    for (size_t i=0; i<tv.size(); ++i) tv[i]=i;
    CHECK_EQUAL(8,tv({3,1,0}));
    tv.index({1,4,8,12});
    for (size_t i=0; i<tv.size(); ++i) tv[i]=i;
    CHECK_EQUAL(2,tv({3,1,0}));
    CHECK(isnan(tv({2,1,0})));
  }
    
  TEST(tensorValAssignment)
  {
    auto arg=std::make_shared<TensorVal>(vector<unsigned>{5,3,2});
    for (size_t i=0; i<arg->size(); ++i) (*arg)[i]=i;
    Scan scan([](double& x,double y,size_t){x+=y;});
    scan.setArgument(arg,"0",0);
    CHECK_EQUAL(arg->rank(), scan.rank());
    CHECK(scan.size()>1);
        
    TensorVal tv;
    tv=scan;

    CHECK_EQUAL(tv.size(), scan.size());
    CHECK_ARRAY_EQUAL(tv.hypercube().dims(), scan.hypercube().dims(), scan.rank());
    for (size_t i=0; i<tv.size(); ++i)
      CHECK_EQUAL(scan[i], tv[i]);
  }

  TEST(permuteAxis)
  {
    // 5x5 example
    Hypercube hc{5,5};
    auto dense=make_shared<TensorVal>(hc);
    for (size_t i=0; i<dense->size(); ++i) (*dense)[i]=i;
    PermuteAxis pa;
    pa.setArgument(dense,"0");
    vector<size_t> permutation{1,4,3};
    pa.setPermutation(permutation);
    CHECK_EQUAL(2, pa.rank());
    CHECK_EQUAL(3, pa.hypercube().dims()[0]);
    CHECK_EQUAL(5, pa.hypercube().dims()[1]);
    CHECK_EQUAL(15, pa.size());
        
    for (size_t i=0; i<pa.size(); ++i)
      {
        switch (i%3)
          {
          case 0:
            CHECK_EQUAL(1, int(pa[i])%5);
            break;
          case 1:
            CHECK_EQUAL(4, int(pa[i])%5);
            break;
          case 2:
            CHECK_EQUAL(3, int(pa[i])%5);
            break;
          }
      }

    pa.setArgument(dense,"1");
    pa.setPermutation(permutation);
    CHECK_EQUAL(2, pa.rank());
    CHECK_EQUAL(5, pa.hypercube().dims()[0]);
    CHECK_EQUAL(3, pa.hypercube().dims()[1]);
    CHECK_EQUAL(15, pa.size());
        
    for (size_t i=0; i<pa.size(); ++i)
      {
        switch (i/5)
          {
          case 0:
            CHECK_EQUAL(1, int(pa[i])/5);
            break;
          case 1:
            CHECK_EQUAL(4, int(pa[i])/5);
            break;
          case 2:
            CHECK_EQUAL(3, int(pa[i])/5);
            break;
          }
      }

        
        
    auto sparse=make_shared<TensorVal>(hc);
    sparse->index(std::set<size_t>{2,4,5,8,10,11,15,20});
    for (size_t i=0; i<sparse->size(); ++i) (*sparse)[i]=sparse->index()[i];

    pa.setArgument(sparse,"0");
    pa.setPermutation(permutation);
    CHECK_EQUAL(2, pa.rank());
    CHECK_EQUAL(3, pa.hypercube().dims()[0]);
    CHECK_EQUAL(5, pa.hypercube().dims()[1]);
    CHECK_EQUAL(3, pa.size());
        
    for (size_t i=0; i<pa.size(); ++i)
      {
        auto splitted=pa.hypercube().splitIndex(pa.index()[i]);
        switch (splitted[0])
          {
          case 0:
            CHECK_EQUAL(1, int(pa[i])%5);
            break;
          case 1:
            CHECK_EQUAL(4, int(pa[i])%5);
            break;
          case 2:
            CHECK_EQUAL(3, int(pa[i])%5);
            break;
          default:
            CHECK(false);
          }
      }
    pa.setArgument(sparse,"1");
    pa.setPermutation(permutation);
    CHECK_EQUAL(2, pa.rank());
    CHECK_EQUAL(3, pa.hypercube().dims()[1]);
    CHECK_EQUAL(5, pa.hypercube().dims()[0]);
    CHECK_EQUAL(4, pa.size());
        
    for (size_t i=0; i<pa.size(); ++i)
      {
        auto splitted=pa.hypercube().splitIndex(pa.index()[i]);
        switch (splitted[1])
          {
          case 0:
            CHECK_EQUAL(1, int(pa[i])/5);
            break;
          case 1:
            CHECK_EQUAL(4, int(pa[i])/5);
            break;
          case 2:
            CHECK_EQUAL(3, int(pa[i])/5);
            break;
          default:
            CHECK(false);
          }
      }
        
  }

  TEST(dimLabels)
  {
    vector<XVector> x{{"x",{Dimension::string,""}}, {"y",{Dimension::string,""}},
                                                      {"z",{Dimension::string,""}}};
    Hypercube hc(x);
    vector<string> expected{"x","y","z"};
    CHECK_EQUAL(expected.size(), hc.dimLabels().size());
    CHECK_ARRAY_EQUAL(expected, hc.dimLabels(), expected.size());
  }

//  struct OuterFixture: public MinskyFixture
//  {
//    VariablePtr x{VariableType::parameter,"x"};
//    VariablePtr y{VariableType::parameter,"y"};
//    VariablePtr z{VariableType::flow,"z"};
//    OperationPtr outer{OperationType::outerProduct};
//    OuterFixture()
//    {
//      model->addItem(x);
//      model->addItem(y);
//      model->addItem(z);
//      model->addItem(outer);
//      auto& xx=x->vValue()->tensorInit;
//      xx.index({1,3});
//      xx.hypercube({5});
//      xx[0]=1;
//      xx[1]=3;
//      y->init("iota(5)");
//    }
//  };
//  
//  TEST_FIXTURE(OuterFixture, sparseOuterProduct)
//    {
//      model->addWire(*x,*outer,1);
//      model->addWire(*x,*outer,2);
//      model->addWire(*outer,*z,1);
//      reset();
//      auto& zz=*z->vValue();
//      CHECK_EQUAL(2, zz.rank());
//      vector<unsigned> expectedIndex={6,8,16,18};
//      CHECK_EQUAL(expectedIndex.size(), zz.size());
//      CHECK_ARRAY_EQUAL(expectedIndex, zz.index(), expectedIndex.size());
//      vector<double> zValues(&zz[0], &zz[0]+zz.size());
//      vector<double> expectedValues={1,3,3,9};
//      CHECK_EQUAL(expectedValues.size(), zz.size());
//      CHECK_ARRAY_EQUAL(expectedValues, zValues, expectedValues.size());
//    }
//  
//
//
//TEST_FIXTURE(OuterFixture, sparse1OuterProduct)
//    {
//      model->addWire(*y,*outer,1);
//      model->addWire(*x,*outer,2);
//      model->addWire(*outer,*z,1);
//      reset();
//      auto& zz=*z->vValue();
//      CHECK_EQUAL(2, zz.rank());
//      vector<unsigned> expectedIndex={5,6,7,8,9,15,16,17,18,19};
//      CHECK_EQUAL(expectedIndex.size(), zz.size());
//      CHECK_ARRAY_EQUAL(expectedIndex, zz.index(), expectedIndex.size());
//      vector<double> zValues(&zz[0], &zz[0]+zz.size());
//      vector<double> expectedValues={0,1,2,3,4,0,3,6,9,12};
//      CHECK_EQUAL(expectedValues.size(), zz.size());
//      CHECK_ARRAY_EQUAL(expectedValues, zValues, expectedValues.size());
//    }
//TEST_FIXTURE(OuterFixture, sparse2OuterProduct)
//    {
//      model->addWire(*x,*outer,1);
//      model->addWire(*y,*outer,2);
//      model->addWire(*outer,*z,1);
//      reset();
//      auto& zz=*z->vValue();
//      CHECK_EQUAL(2, zz.rank());
//      vector<unsigned> expectedIndex={1,3,6,8,11,13,16,18,21,23};
//      CHECK_EQUAL(expectedIndex.size(), zz.size());
//      CHECK_ARRAY_EQUAL(expectedIndex, zz.index(), expectedIndex.size());
//      vector<double> zValues(&zz[0], &zz[0]+zz.size());
//      vector<double> expectedValues={0,0,1,3,2,6,3,9,4,12};
//      CHECK_EQUAL(expectedValues.size(), zz.size());
//      CHECK_ARRAY_EQUAL(expectedValues, zValues, expectedValues.size());
//    }
//
// TEST_FIXTURE(TestFixture,TensorVarValAssignment)
//   {
//     auto ev=make_shared<EvalCommon>();
//     double fv[100], sv[10];
//     ev->update(fv,sizeof(fv)/sizeof(fv[0]),sv);
//     auto startTimestamp=ev->timestamp();
//     TensorVarVal tvv(to->vValue(),ev); 
//     tvv=fromVal;
//     CHECK_EQUAL(fromVal.rank(), tvv.rank());
//     CHECK_ARRAY_EQUAL(fromVal.shape().data(), tvv.shape().data(), fromVal.rank());
//     CHECK_ARRAY_CLOSE(&fromVal[0], &tvv[0], fromVal.size(), 1e-5);
//     
//     CHECK(ev->timestamp()>startTimestamp);
//     CHECK_EQUAL(fv,ev->flowVars());
//     CHECK_EQUAL(sv,ev->stockVars());
//     CHECK_EQUAL(sv,ev->stockVars());
//     CHECK_EQUAL(sizeof(fv)/sizeof(fv[0]),ev->fvSize());
//   }
 
}
