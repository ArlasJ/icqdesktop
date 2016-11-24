#pragma once

#include "../../../corelib/collection_helper.h"

namespace core
{
    namespace wim
    {
        struct chat_member_info
        {
            std::string aimid_;
            std::string role_;
            std::string first_name_;
            std::string last_name_;
            std::string nick_name_;

            bool friend_;
            bool no_avatar_;

            chat_member_info()
                : friend_(false)
                , no_avatar_(false)
            {
            }
        };

        class chat_info
        {
            std::string aimid_;
            std::string name_;
            std::string about_;
            std::string your_role_;
            std::string owner_;
            std::string members_version_;
            std::string info_version_;
            std::string location_;
            std::string stamp_;
            std::string creator_;
            std::string default_role_;

            uint32_t create_time_;
            uint32_t members_count_;
            uint32_t friend_count_;
            uint32_t blocked_count_;
            uint32_t pending_count_;

            bool you_blocked_;
            bool you_pending_;
            bool you_member_;
            bool public_;
            bool live_;
            bool controlled_;
            bool joinModeration_;
            bool age_restriction_;

            std::list<chat_member_info> members_;

        public:
            chat_info();

            int32_t unserialize(const rapidjson::Value& _node);
            void serialize(core::coll_helper _coll) const;
        };

    }
}