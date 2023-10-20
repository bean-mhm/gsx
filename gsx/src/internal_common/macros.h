#pragma once

#define no_default_construct(CLASS) CLASS() = delete
#define no_copy_construct(CLASS) CLASS(const CLASS&) = delete
#define no_assignment(CLASS) CLASS& operator= (const CLASS&) = delete
#define no_copy_construct_no_assignment(CLASS) no_copy_construct(CLASS); no_assignment(CLASS);
#define no_default_copy_construct_no_assignment(CLASS) no_default_construct(CLASS); \
no_copy_construct(CLASS); no_assignment(CLASS);
