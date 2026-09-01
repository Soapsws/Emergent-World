#pragma once

// Uniform access for pools whose transform is stored either directly or in
// an agent State object.
template <typename Pool>
/*
decltype(auto) lets the compiler deduce the return type
this is useful because cells have states whereas food and roots do not, so there are different paths to access transform
*/
decltype(auto) TransformAt(Pool& pool, int index) {
    if constexpr (requires { pool.state[index].transform; }) {
        return (pool.state[index].transform);
    } else {
        return (pool.transform[index]);
    }
}
