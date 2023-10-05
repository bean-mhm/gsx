#pragma once

// ecs
#include "internal_ecs/log.h"
#include "internal_ecs/event.h"
#include "internal_ecs/system.h"
#include "internal_ecs/world.h"
namespace ecs = tef::ecs;

// math
#include "internal_math/vec2.h"
#include "internal_math/vec3.h"
#include "internal_math/vec4.h"
#include "internal_math/quaternion.h"
#include "internal_math/bounds2.h"
#include "internal_math/bounds3.h"
#include "internal_math/ray.h"
#include "internal_math/matrix.h"
#include "internal_math/transform.h"
#include "internal_math/prng.h"
#include "internal_math/utils.h"
namespace math = tef::math;

// spatial
#include "internal_spatial/grid_2d.h"
#include "internal_spatial/hash_grid_2d.h"
#include "internal_spatial/quadtree.h"
#include "internal_spatial/octree.h"
namespace spatial = tef::spatial;

// str
#include "internal_str/utils.h"
namespace str = tef::str;

// misc
#include "internal_misc/fixed_vector.h"
#include "internal_misc/worker.h"
#include "internal_misc/utils.h"
namespace misc = tef::misc;
