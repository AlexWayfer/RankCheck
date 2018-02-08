#ifndef SRC_CLIENT_RANKCHECK_USERNAMELOOKUP_HPP_
#define SRC_CLIENT_RANKCHECK_USERNAMELOOKUP_HPP_

#include <SFML/Config.hpp>
#include <Shared/Utils/Timer.hpp>
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <Shared/External/MinGWThreads/MinGWThreadsAuto.hpp>

namespace sf
{
class String;
}

class UsernameLookup
{
public:
	UsernameLookup();
	~UsernameLookup();

	using Callback = std::function<void(std::string)>;

	void lookup(sf::Uint64 steamID, Callback callback);
	std::string getCachedName(sf::Uint64 steamID) const;
	void process();

private:

	static std::string parseResponse(const std::string & response);

	struct Connection
	{
		sf::Uint64 steamID;
		std::atomic_bool done = ATOMIC_VAR_INIT(false);
		std::thread thread;
		std::string result;
		Callback callback;
	};

	struct CacheEntry
	{
		std::string name;
		Countdown timer;
	};
	std::map<sf::Uint64, CacheEntry> cache;
	std::vector<std::unique_ptr<Connection> > connections;
};

#endif