	// Reference to another package. 
	class package_ref
		{
		private:
			hash hash_m;

		public:
			// ctors/dtor
			package_ref() noexcept : hash_m( hash_zero ) {}
			package_ref( const hash &_hash ) noexcept : hash_m( _hash ) {}
			package_ref( const package_ref &other ) noexcept : hash_m( other.hash_m ) {}
			package_ref &operator=( const package_ref &other ) noexcept { this->hash_m = other.hash_m; return *this; }
			package_ref( package_ref &&other ) noexcept : hash_m( other.hash_m ) { other.hash_m = hash_zero; }
			package_ref &operator=( package_ref &&other ) noexcept { this->hash_m = other.hash_m; other.hash_m = hash_zero; return *this; }
			~package_ref() = default;

			// comparing & sorting
			bool operator==( const package_ref &rval ) const { return this->hash_m == rval.hash_m; }
			bool operator!=( const package_ref &rval ) const { return this->hash_m != rval.hash_m; }
			bool operator<( const package_ref &rval ) const { return this->hash_m < rval.hash_m; }

			// conversions to boolean (behaves like a pointer, true if non-null) 
			operator bool() const { return this->hash_m != hash_zero; }
			operator hash() const { return this->hash_m; }

			// returns the "null" package_ref value 
			static const package_ref &null() { static package_ref null_id; return null_id; }
		};

	inline std::ostream &operator<<( std::ostream &os, const package_ref &ref )
		{
		return os << hash( ref );
		}

	const package_ref package_ref_zero = package_ref( hash_zero );
	const package_ref package_ref_inf = package_ref( hash_inf );
	const package_ref package_ref_sup = package_ref( hash_sup );

