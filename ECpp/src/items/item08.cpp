/** Item 8: Prevent exceptions from leaving destructors. */

#include <iostream>
#include <exception>

/** the exception has to come from some non-destructor function. */

class DBConnection
{
public:
	static DBConnection create() {};

	void close() { throw new std::exception(); };
};

class DBConn
{
public:
	DBConn(const DBConnection& newDB)
		: db(newDB)
		, closed(false)
	{
	}

	~DBConn()
	{
		if (!closed)
		{
			try
			{
				close();
			}
			catch (std::exception e)
			{
				// std::abort(); // Terminate the program
				// std::cerr << "error" << std::endl; // Swallow the excetion
			}
		}
	}

	void close()
	{
		db.close();
		closed = true;
	}

private:
	DBConnection db;
	bool closed;
};

void useDB()
{
	DBConn dbc(DBConnection::create());
}

void testDB()
{
	useDB();
}

/** Things to Remember
	 * Destructors should never emit exceptions. If functions called in a destructor may throw, the
       destructor should catch any exceptions, then swallow them or terminate the program.
       
	 * If class clients need to be able to react to exceptions thrown during an operation, the class should
       provide a regular (i.e., non-destructor) function that performs the operation.
*/
