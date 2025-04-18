/**
 * @file main.cpp
 * @author Wei Li (weili_yzzcq@163.com)
 * @brief Fetching emails using IMAP
 * @version 0.1
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "curl_easy.h"
#include "curl_exception.h"

#include <iostream>
#include <string>
#include <string_view>

/**
 * @brief Fetching emails using IMAP
 * 
 * Write a program that can get information from an email server using IMAP. 
 * The program should be able to:
 * 1. Get a list of folders from the mailbox
 * 2. Get unread emails from a particular folder
 * 
 */

/**
 * @brief Solution: Asio C++ Library
 https://think-async.com/Asio/
 https://github.com/chriskohlhoff/asio/
------------------------------------------------------ */
class imap_connection
{
public:
    imap_connection(std::string_view url, const unsigned short port, std::string_view user, std::string_view pass)
        : url(url)
        , port(port)
        , user(user)
        , pass(pass)
    {
    }

    std::string get_folders()
    {
        std::stringstream str;
        try
        {
            curl::curl_ios<std::stringstream> writer(str);

            curl::curl_easy easy(writer);
            easy.add<CURLOPT_URL>(url.data());
            setup_easy(easy);

            easy.perform();
        }
        catch (const curl::curl_easy_exception &error)
        {
            auto errors = error.get_traceback();
            error.print_traceback();
        }

        return str.str();
    }

    std::string examine_folder(std::string_view folder)
    {
        std::stringstream str;
        try
        {
            curl::curl_ios<std::stringstream> writer(str);

            curl::curl_easy easy(writer);
            easy.add<CURLOPT_URL>(url.data());
            easy.add<CURLOPT_CUSTOMREQUEST>((std::string("EXAMINE ") + folder.data()).c_str());
            setup_easy(easy);

            easy.perform();
        }
        catch (const curl::curl_easy_exception &error)
        {
            auto errors = error.get_traceback();
            error.print_traceback();
        }

        return str.str();
    }

    std::vector<unsigned int> fetch_unread_uids(std::string_view folder)
    {
        std::stringstream str;

        try
        {
            curl::curl_ios<std::stringstream> writer(str);

            curl::curl_easy easy(writer);
            easy.add<CURLOPT_URL>((url.data() + std::string("/") + folder.data() + std::string("/")).c_str());
            easy.add<CURLOPT_CUSTOMREQUEST>("SEARCH UNSEEN");
            setup_easy(easy);

            easy.perform();
        }
        catch (const curl::curl_easy_exception &error)
        {
            auto errors = error.get_traceback();
            error.print_traceback();
        }

        std::vector<unsigned int> uids;
        str.seekg(8, std::ios::beg);
        unsigned int uid;
        while (str >> uid) uids.push_back(uid);

        return uids;
    }

    std::string fetch_email(std::string_view folder, unsigned int uid)
    {
        std::stringstream str;

        try
        {
            curl::curl_ios<std::stringstream> writer(str);

            curl::curl_easy easy(writer);
            easy.add<CURLOPT_URL>(
                (url.data() + std::string("/") + folder.data() + std::string("/;UID=") + std::to_string(uid)).c_str());
            setup_easy(easy);

            easy.perform();
        }
        catch (curl::curl_easy_exception error)
        {
            auto errors = error.get_traceback();
            error.print_traceback();
        }

        return str.str();
    }

private:
    void setup_easy(curl::curl_easy &easy)
    {
        easy.add<CURLOPT_PORT>(port);
        easy.add<CURLOPT_USERNAME>(user.c_str());
        easy.add<CURLOPT_PASSWORD>(pass.c_str());
        easy.add<CURLOPT_USE_SSL>(CURLUSESSL_ALL);
        easy.add<CURLOPT_SSL_VERIFYPEER>(0L);
        easy.add<CURLOPT_SSL_VERIFYHOST>(0L);
        easy.add<CURLOPT_USERAGENT>("libcurl-agent/1.0");
    }

private:
    std::string    url;
    unsigned short port;
    std::string    user;
    std::string    pass;
};

// ------------------------------
int main(int argc, char **argv)
{
    imap_connection imap("imaps://imap.gmail.com", 993, "...(your username)...", "...(your password)...");

    auto folders = imap.get_folders();
    std::cout << folders << std::endl;

    auto info = imap.examine_folder("inbox");
    std::cout << info << std::endl;

    auto uids = imap.fetch_unread_uids("inbox");

    if (!uids.empty())
    {
        auto email = imap.fetch_email("inbox", uids.back());
        std::cout << email << std::endl;
    }

    return 0;
}
