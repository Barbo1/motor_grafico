#pragma once

#include "./physical.hpp"
#include "../pr_objects/square.hpp"
#include "../pr_objects/circle.hpp"
#include "../pr_objects/nedge.hpp"
#include "../pr_objects/line.hpp"
#include "../pr_objects/particle.hpp"


/* take two objects and verifies if they have collide. */
bool test_collition (Physical&, Physical&);
bool test_collition (Circle&, Circle&);
bool test_collition (Square&, Square&);
bool test_collition (Square&, Circle&);
bool test_collition (Circle&, Square&);
bool test_collition (Circle&, NEdge&);
bool test_collition (Square&, NEdge&);
bool test_collition (NEdge&, NEdge&);
bool test_collition (NEdge&, Circle&);
bool test_collition (NEdge&, Square&);

bool test_collition (Physical&, Line&);
bool test_collition (Square&, Line&);
bool test_collition (Circle&, Line&);
bool test_collition (NEdge&, Line&);

bool test_collition (Particle&, Physical&);
bool test_collition (Particle&, Square&);
bool test_collition (Particle&, Circle&);
bool test_collition (Particle&, NEdge&);


/* take two objects and generate the collition. */
void resolve_collition (Physical&, Physical&);
void resolve_collition (Circle&, Circle&);
void resolve_collition (Square&, Square&);
void resolve_collition (Square&, Circle&);
void resolve_collition (Circle&, Square&);
void resolve_collition (Circle&, NEdge&);
void resolve_collition (Square&, NEdge&);
void resolve_collition (NEdge&, NEdge&);
void resolve_collition (NEdge&, Circle&);
void resolve_collition (NEdge&, Square&);

void resolve_collition (Physical&, Line&);
void resolve_collition (Square&, Line&);
void resolve_collition (Circle&, Line&);
void resolve_collition (NEdge&, Line&);

void resolve_collition (Particle&, Physical&);
void resolve_collition (Particle&, Square&);
void resolve_collition (Particle&, Circle&);
void resolve_collition (Particle&, NEdge&);


/* Given that a collition had ocurred, this function corrects the superposition of the
 * objects moving the first slightly, so that the only point in it is the one returned
 * by collition_point.
 * */
void correct_collition (Physical&, Physical&);
void correct_collition (Circle&, Circle&);
void correct_collition (Square&, Square&);
void correct_collition (Square&, Circle&);
void correct_collition (Circle&, Square&);
void correct_collition (Circle&, NEdge&);
void correct_collition (Square&, NEdge&);
void correct_collition (NEdge&, NEdge&);
void correct_collition (NEdge&, Circle&);
void correct_collition (NEdge&, Square&);

void correct_collition (Physical&, Line&);
void correct_collition (Square&, Line&);
void correct_collition (Circle&, Line&);
void correct_collition (NEdge&, Line&);

void correct_collition (Particle&, Physical&);
void correct_collition (Particle&, Square&);
void correct_collition (Particle&, Circle&);
void correct_collition (Particle&, NEdge&);


/* Returns the point of the collition between two objects. */
Dir2 collition_point (Physical&, Physical&);
Dir2 collition_point (Square&, Square&);
Dir2 collition_point (Square&, Circle&);
Dir2 collition_point (Circle&, Square&);
Dir2 collition_point (Square&, Circle&);
Dir2 collition_point (Circle&, NEdge&);
Dir2 collition_point (Square&, NEdge&);
Dir2 collition_point (NEdge&, NEdge&);
Dir2 collition_point (NEdge&, Circle&);
Dir2 collition_point (NEdge&, Square&);

Dir2 collition_point (Physical&, Line&);
Dir2 collition_point (Square&, Line&);
Dir2 collition_point (Circle&, Line&);
Dir2 collition_point (NEdge&, Line&);
