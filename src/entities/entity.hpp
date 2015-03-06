/**
 *
 */

#ifndef RPEC__ENTITIES__ENTITY__H_
#define RPEC__ENTITIES__ENTITY__H_

#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <rxcpp/rx.hpp>

namespace rpec {
namespace entities {

class Entity
{
public:
    typedef boost::uuids::uuid id_type;
    typedef boost::hash<boost::uuids::uuid> id_hash;

    Entity ()
    : m_id(boost::uuids::random_generator()())
    { }

    virtual ~Entity()
    { }

    id_type const & id() const {
        return m_id;
    }

private:
    id_type m_id;
};

} // end namespace entities
} // end namespace rpec

#endif // RPEC__ENTITIES__ENTITY__H_

