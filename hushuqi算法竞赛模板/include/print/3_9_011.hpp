#include <ext/pb_ds/assoc_container.hpp>
#include "support/base.hpp"

namespace pbds_hash
{
using namespace __gnu_pbds;

template <class K, class V> using HashMap = gp_hash_table<K, V, CustomHash>;
}
