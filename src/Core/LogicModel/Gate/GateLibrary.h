/**
 * This file is part of the IC reverse engineering tool Degate.
 *
 * Copyright 2008, 2009, 2010 by Martin Schobert
 * Copyright 2012 Robert Nitsch
 * Copyright 2019-2020 Dorian Bachelot
 *
 * Degate is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Degate is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with degate. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __GATELIBRARY_H__
#define __GATELIBRARY_H__

#include "Core/Primitive/DeepCopyable.h"
#include "GateTemplatePort.h"

#include <set>
#include <map>
#include <sstream>
#include "Globals.h"

namespace degate
{
    /**
     * This class represents a library of template cells.
     *
     * @todo There is no way to deal with gate libraries, which
     *   have different image sizes for the gate templates than the
     *   current project. There should be a way to scale the template
     *   images, but it is unknown how to derive the scaling factor without pain.
     */
    class GateLibrary : public DeepCopyable
    {
    public:

        typedef std::map<object_id_t, GateTemplate_shptr> gate_lib_collection_t;
        typedef gate_lib_collection_t::iterator template_iterator;
        typedef gate_lib_collection_t::const_iterator const_template_iterator;

    private:

        gate_lib_collection_t templates;

    public:

        /**
         * Constructor for the gate library.
         */
        GateLibrary();

        /**
         * The dtor.
         */
        virtual ~GateLibrary();

        //@{
        DeepCopyable_shptr clone_shallow() const;
        void clone_deep_into(DeepCopyable_shptr destination, oldnew_t* oldnew) const;
        //@}

        /**
         * Remove a template from the gate library.
         */
        void remove_template(GateTemplate_shptr gate_template);

        /**
         * Add a template to the library.
         * @exception InvalidObjectIDException This exception is thrown if the
         *   template has no object ID.
         * @exception InvalidPointerException
         */
        void add_template(GateTemplate_shptr gate_template);

        /**
         * Get a gate template from the library.
         * @exception CollectionLookupException This exception is thrown if
         *  there is no gate template that has ID \p id.
         * @exception InvalidObjectIDException This exception is thrown if the
         *   object ID is invalid.
         * @return Returns a shared pointer to the template. The
         *   pointer value is nullptr, if a template with the \p id
         *   was not found.
         */
        GateTemplate_shptr get_template(object_id_t id);


        /**
         * Check if a template for a given \p id exists.
         */
        bool exists_template(object_id_t id) const;

        /**
         * Check for a name in the gate library.
         * @return Returns true, if a template name is already used for a template.
         */
        bool is_name_in_use(std::string const& name) const;


        /**
         * Check if there is a template port in the gate library with the specified object ID.
         */
        bool exists_template_port(object_id_t port_id);

        /**
         * Lookup a template port in the gate library.
         * @throws CollectionLookupException Throws this exception, if the port was nout found.
         */
        GateTemplatePort_shptr get_template_port(object_id_t port_id);


        /**
         * Get an iterator in order to iterate over gate templates.
         */
        template_iterator begin();

        /**
         * Get the end marker for the iteration.
         */
        template_iterator end();

        /**
         * Get an iterator in order to iterate over gate templates.
         */
        const_template_iterator begin() const;

        /**
         * Get the end marker for the iteration.
         */
        const_template_iterator end() const;

        /**
         * print the gate library.
         */
        void print(std::ostream& os);
    };
}

#endif
