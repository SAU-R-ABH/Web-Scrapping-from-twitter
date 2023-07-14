#include <iostream>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

void scrape_twitter_data(const std::string& username) {
    CURL* curl;
    CURLcode res;

    std::string url = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=" + username + "&count=10";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Authorization: Bearer YOUR_BEARER_TOKEN");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            rapidjson::Document document;
            document.Parse(response.c_str());

            if (document.IsArray()) {
                for (rapidjson::SizeType i = 0; i < document.Size(); ++i) {
                    const rapidjson::Value& tweet = document[i];
                    if (tweet.HasMember("text") && tweet.HasMember("created_at")) {
                        const rapidjson::Value& text = tweet["text"];
                        const rapidjson::Value& created_at = tweet["created_at"];
                        std::cout << "Text: " << text.GetString() << std::endl;
                        std::cout << "Date: " << created_at.GetString() << std::endl;
                        std::cout << std::endl;
                    }
                }
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
}

int main() {
    std::string username;
    std::cout << "Enter Twitter Username: ";
    std::cin >> username;

    scrape_twitter_data(username);

    return 0;
}
