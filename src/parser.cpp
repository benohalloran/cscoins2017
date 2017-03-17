#include "solver.hpp"

#include "rapidjson/document.h"

using namespace rapidjson;

static int from_hex(char c)
{
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return c - '0';
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
        return c - ('a' - 10);
    default:
        assert(0);
    }
}

void parse(const char *json, challenge &c)
{
    Document d;
    d.Parse(json);
    assert(d.IsObject());

    assert(d.HasMember("challenge_id"));
    const Value &id = d["challenge_id"];
    assert(id.IsInt());
    c.id = id.GetInt();

    assert(d.HasMember("challenge_name"));
    const Value &name = d["challenge_name"];
    assert(name.IsString());
    const char *name_str = name.GetString();
    if (!strcmp(name_str, "sorted_list")) {
        c.type = TYPE_SORTED_LIST;
    } else if (!strcmp(name_str, "reverse_sorted_list")) {
        c.type = TYPE_REVERSE_SORTED_LIST;
    } else if (!strcmp(name_str, "shortest_path")) {
        c.type = TYPE_SHORTEST_PATH;
    } else {
        assert(0);
    }

    assert(d.HasMember("last_solution_hash"));
    const Value &hash = d["last_solution_hash"];
    assert(hash.IsString());
    const char *hash_str = hash.GetString();
    assert(strlen(hash_str) == 64);
    for (unsigned int i = 0; i < 64; ++i) {
        assert((hash_str[i] >= '0' && hash_str[i] <= '9')
            || (hash_str[i] >= 'a' && hash_str[i] <= 'f'));
    }
    strcpy(c.prev_hash, hash_str);

    assert(d.HasMember("hash_prefix"));
    const Value &hash_prefix = d["hash_prefix"];
    assert(hash_prefix.IsString());
    const char *hash_prefix_str = hash_prefix.GetString();
    c.hash_prefix_len = strlen(hash_prefix_str);
    assert(c.hash_prefix_len <= 64);
    for (unsigned int i = 0; i < c.hash_prefix_len; ++i) {
        assert((hash_prefix_str[i] >= '0' && hash_prefix_str[i] <= '9')
            || (hash_prefix_str[i] >= 'a' && hash_prefix_str[i] <= 'f'));
    }
    strcpy(c.hash_prefix, hash_prefix_str);

    c.prefix4 = 0;
    if (c.hash_prefix_len >= 4) {
        c.prefix4 = from_hex(hash_prefix_str[1]);
        c.prefix4 |= from_hex(hash_prefix_str[0]) << 4;
        c.prefix4 |= from_hex(hash_prefix_str[2]) << 12;
        c.prefix4 |= from_hex(hash_prefix_str[3]) << 8;
    }

    assert(d.HasMember("parameters"));
    const Value &parameters = d["parameters"];
    assert(parameters.IsObject());

    Value::ConstMemberIterator nb_elements_iter =
        parameters.FindMember("nb_elements");
    if (nb_elements_iter != parameters.MemberEnd()) {
        const Value &nb_elements = nb_elements_iter->value;
        assert(nb_elements.IsUint());
        c.n = nb_elements.GetUint();
        assert(c.n >= 1);
    } else {
        assert(parameters.HasMember("grid_size"));
        const Value &grid_size = parameters["grid_size"];
        assert(grid_size.IsUint());
        c.n = grid_size.GetUint();
        assert(c.n >= 4);

        assert(parameters.HasMember("nb_blockers"));
        const Value &nb_blockers = parameters["nb_blockers"];
        assert(nb_blockers.IsUint());
        c.m = nb_blockers.GetUint();
    }
}
