	// Reference to an element within the same package. Acts like a memory handle. 
	// It can be created, held and copied, but cannot be set to a specific value other than null.
	class entity_ref
		{
		private:
			uuid id_m;

			// set entity reference, only allow this internally and from EntityReader
			entity_ref( const uuid &id ) : id_m( id ) {};
			friend class EntityReader;

		public:
			// ctors/dtor
			entity_ref() noexcept : id_m( uuid_zero ) {}
			entity_ref( const entity_ref &other ) noexcept : id_m( other.id_m ) {}
			entity_ref &operator=( const entity_ref &other ) noexcept { this->id_m = other.id_m; return *this; }
			entity_ref( entity_ref &&other ) noexcept : id_m( other.id_m ) { other.id_m = uuid_zero; }
			entity_ref &operator=( entity_ref&& other ) noexcept { this->id_m = other.id_m; other.id_m = uuid_zero; return *this; }
			~entity_ref() = default;

			// make a new reference with a unique uuid
			static entity_ref make_ref();

			// comparing & sorting
			bool operator==( const entity_ref &rval ) const { return this->id_m == rval.id_m; }
			bool operator!=( const entity_ref &rval ) const { return this->id_m != rval.id_m; }
			bool operator<( const entity_ref &rval ) const { return this->id_m < rval.id_m; }

			// conversions to boolean (behaves like a pointer, true if non-null) 
			operator bool() const { return this->id_m != uuid_zero; }
			operator uuid() const { return this->id_m; }

			// returns the "null" entity_ref value 
			static const entity_ref &null() { static entity_ref null_id; return null_id; }
			
			// inf and sup values, for comparing
			static entity_ref &_inf() { static entity_ref inf_id(uuid_inf); return inf_id; }
			static entity_ref &_sup() { static entity_ref sup_id(uuid_sup); return sup_id; }
		};

	inline std::ostream &operator<<( std::ostream &os, const entity_ref &ref )
		{
		return os << uuid( ref );
		}

	const entity_ref entity_ref_zero = entity_ref::null();
	const entity_ref entity_ref_inf = entity_ref::_inf();
	const entity_ref entity_ref_sup = entity_ref::_sup();

