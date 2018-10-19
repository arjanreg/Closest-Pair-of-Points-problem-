/**
 * algorithm EfficientClosestPair from Levitin 3rd p194
 * closest pair by divide and conquer
 * @author Jon Beck
 * @version 4 March 2017
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>

using namespace std;

typedef unsigned int uint;

/**
 * Make a class that sorts ascending by the first element of a pair
 */
struct SortFirst
{
  bool operator()(const pair<double,double> & left,
                  const pair<double,double> & right)
  {
    if( left.first == right.first )
      return left.second < right.second;
    return left.first < right.first;
  }
};

/**
 * Make a class that sorts ascending by the second element of a pair
 */
struct SortSecond
{
  bool operator()(const pair<double,double> & left,
                  const pair<double,double> & right)
  {
    if( left.second == right.second )
      return left.first < right.first;
    return left.second < right.second;
  }
};

/**
 * simple cartesian distance between two points imlemented
 * as pairs of doubles
 */
double distance( const pair<double,double> & p1,
                 const pair<double,double> & p2 )
{
  double diff_xs = p1.first - p2.first;
  double diff_ys = p1.second - p2.second;
  return sqrt( diff_xs * diff_xs + diff_ys * diff_ys );
}

/**
 * divide and conquer closest pair
 * if the number of elements is less than 4, solve by brute force
 * @param p a vector of points modeled as pairs of doubles, in
 * ascending order by the x coordinate
 * @param q a vector of points modeled as pairs of doubles, in
 * ascending order by the y coordinate
 * @return the Cartesian distance between the closest pair of points
 */
double dc_close_pair_dist( const vector<pair<double,double>> & p,
                           const vector<pair<double,double>> & q, 
                           uint & count_ops )
{
  if( p.size() < 2 )
    return 0.0;
  if( p.size() == 2 )
    return distance( p.at( 0 ), p.at( 1 ));
  if( p.size() == 3 )
  {
    double dist01 = distance( p.at( 0 ), p.at( 1 ));
    double dist02 = distance( p.at( 0 ), p.at( 2 ));
    double dist12 = distance( p.at( 1 ), p.at( 2 ));
    double min01 = dist01 < dist02 ? dist01 : dist02;
    return min01 < dist12 ? min01 : dist12;
  }

  vector<pair<double,double>> pl;
  vector<pair<double,double>> pr;
  vector<pair<double,double>> ql;
  vector<pair<double,double>> qr;

  for( uint i = 0; i < p.size() / 2; i++ )
  {
    count_ops++;
    pl.push_back( p.at( i ));
    ql.push_back( p.at( i ));
  }

  for( uint i = p.size() / 2; i < p.size(); i++ )
  {
    count_ops++; 
    pr.push_back( p.at( i ));
    qr.push_back( p.at( i ));
  }

  // pl and pr and in order, but need to sort ql and qr
  sort( ql.begin(), ql.end(), SortSecond() );
  sort( qr.begin(), qr.end(), SortSecond() );

  double dl = dc_close_pair_dist( pl, ql, count_ops );
  double dr = dc_close_pair_dist( pr, qr, count_ops );
  double d = min( dl, dr );
  double m = p.at( p.size() / 2 - 1 ).first;

  vector<pair<double,double>> s;
  for( auto point : q )
  {
    //count_ops++;
    if( abs( point.first - m ) < d )
    {
      s.push_back( point );
    }
  }

  double dminsq = d * d;
  if( s.size() == 0 )
  {
    return sqrt( dminsq );
  }

  for( uint i = 0; i < s.size() - 1; i++ )
  {
    uint k = i + 1;
    while( k < s.size() &&
           pow( s.at( k ).second - s.at( i ).second, 2.0 ) < dminsq )
    { 
      count_ops++; 
      dminsq = min( pow( s.at( k ).first - s.at( i ).first, 2.0 ) +
                    pow( s.at( k ).second - s.at( i ).second, 2.0 ),
                    dminsq );
      k++;
    }
  }
  return sqrt( dminsq );
}

/**
 * brute force closest pair
 * @param p a vector of points modeled as pairs of doubles, in
 * ascending order by the x coordinate
 * @param q a vector of points modeled as pairs of doubles, in
 * ascending order by the y coordinate
 * @return the Cartesian distance between the closest pair of points
 */
double bf_close_pair_dist( const vector<pair<double,double>> & p,
                           const vector<pair<double,double>> & q,
                           uint & count_ops )
{ 
  double dmin = DBL_MAX; 
  uint size = p.size(); 

  for( uint i = 0; i < (size - 1); i++ )
  {
    for( uint j = i + 1; j < size; j++ )
    {
      count_ops++;  
      dmin = min(dmin, distance( p.at(i), p.at(j) ));
    }
  }
  return dmin; 
}

int main()
{
  vector<pair<double, double>> p; // Levitin's P array
  vector<pair<double, double>> q; // Levitin's Q array

  while( !cin.eof() )
  {
    double xvalue;
    double yvalue;

    cin >> xvalue >> yvalue;
    if( !cin.eof() )
    {
      p.push_back( make_pair( xvalue, yvalue ));
      q.push_back( make_pair( xvalue, yvalue ));
    }
  }
  sort( p.begin(), p.end(), SortFirst() );
  sort( q.begin(), q.end(), SortSecond() );

  uint count_bf = 0; 
  uint count_dc = 0; 

  double mindist_bf = bf_close_pair_dist( p, q, count_bf ); 
  double mindist = dc_close_pair_dist( p, q, count_dc );

  cout << "Minimum bf distance: " << mindist_bf << endl; 
  cout << "Minimum dc distance: " << mindist << endl;
  cerr << p.size() << '\t' << count_bf << '\t' << count_dc << endl; 
  {
    
  }
  return 0;
}
