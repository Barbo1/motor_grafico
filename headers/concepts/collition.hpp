#pragma once

#include "./physical.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/nedge.hpp"
#include "../pr_objects/line.hpp"
#include "../pr_objects/particle.hpp"


/* Take two objects and verifies if they have collide. The 
 * order of the parameters are from least to most complex. 
 * */
bool test_collition (const Line&, Physical&);
bool test_collition (const Line&, const Particle&);
bool test_collition (const Line&, const Circle&);
bool test_collition (const Line&, const Square&);
template<std::size_t N> bool test_collition (const Line&, const NEdge<N>&);

bool test_collition (const Particle&, Physical&);
bool test_collition (const Particle&, const Circle&);
bool test_collition (const Particle&, const Square&);
template<std::size_t N> bool test_collition (const Particle&, const NEdge<N>&);

bool test_collition (Physical&, Physical&);
bool test_collition (const Circle&, const Circle&);
bool test_collition (const Circle&, const Square&);
bool test_collition (const Square&, const Square&);
template<std::size_t N> bool test_collition (const Circle&, const NEdge<N>&);
template<std::size_t N> bool test_collition (const Square&, const NEdge<N>&);
template<std::size_t N, std::size_t M> bool test_collition (const NEdge<N>&, const NEdge<M>&);



/* Take two objects and generate the collition. The order 
 * of the parameters are from least to most complex. 
 * */
void resolve_collition (Physical&, Line&);
void resolve_collition (Particle&, Line&);
void resolve_collition (Circle&, Line&);
void resolve_collition (Square&, Line&);
template<std::size_t N> void resolve_collition (NEdge<N>&, Line&);

void resolve_collition (Particle&, Physical&);
void resolve_collition (Particle&, Square&);
void resolve_collition (Particle&, Circle&);
template<std::size_t N> void resolve_collition (Particle&, NEdge<N>&);

void resolve_collition (Physical&, Physical&);
void resolve_collition (Circle&, Circle&);
void resolve_collition (Circle&, Square&);
void resolve_collition (Square&, Circle&);
void resolve_collition (Square&, Square&);
template<std::size_t N> void resolve_collition (Circle&, NEdge<N>&);
template<std::size_t N> void resolve_collition (Square&, NEdge<N>&);
template<std::size_t N> void resolve_collition (NEdge<N>&, Circle&);
template<std::size_t N> void resolve_collition (NEdge<N>&, Square&);
template<std::size_t N, std::size_t M> void resolve_collition (NEdge<N>&, NEdge<M>&);



/* Given that a collition had ocurred, this function corrects 
 * the superposition of the objects moving the first slightly, 
 * so that the only point in it is the one returned by 
 * collition_point.
 * */

void correct_collition (Physical&, Line&);
void correct_collition (Particle&, Line&);
void correct_collition (Square&, Line&);
void correct_collition (Circle&, Line&);
template<std::size_t N> void correct_collition (NEdge<N>&, Line&);

void correct_collition (Particle&, Physical&);
void correct_collition (Particle&, Square&);
void correct_collition (Particle&, Circle&);
template<std::size_t N> void correct_collition (Particle&, NEdge<N>&);

void correct_collition (Physical&, Physical&);
void correct_collition (Circle&, Circle&);
void correct_collition (Circle&, Square&);
void correct_collition (Square&, Circle&);
void correct_collition (Square&, Square&);
template<std::size_t N> void correct_collition (Circle&, NEdge<N>&);
template<std::size_t N> void correct_collition (Square&, NEdge<N>&);
template<std::size_t N> void correct_collition (NEdge<N>&, Circle&);
template<std::size_t N> void correct_collition (NEdge<N>&, Square&);
template<std::size_t N, std::size_t M> void correct_collition (NEdge<N>&, NEdge<M>&);


/* Returns the point of the collition between two objects. */
Dir2 collition_point (Line&, Physical&);
Dir2 collition_point (Line&, Square&);
Dir2 collition_point (Line&, Circle&);
template<std::size_t N> Dir2 collition_point (Line&, NEdge<N>&);

Dir2 collition_point (Physical&, Physical&);
Dir2 collition_point (Circle&, Circle&);
Dir2 collition_point (Circle&, Square&);
Dir2 collition_point (Square&, Square&);
template<std::size_t N> Dir2 collition_point (Circle&, NEdge<N>&);
template<std::size_t N> Dir2 collition_point (Square&, NEdge<N>&);
template<std::size_t N, std::size_t M> Dir2 collition_point (NEdge<N>&, NEdge<M>&);
