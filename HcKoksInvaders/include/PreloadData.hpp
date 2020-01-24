#pragma once
#include <memory>
#include <string>

struct PreloadData {
	std::unique_ptr<uint8_t> m_data;
	size_t m_dataLength;
	std::string m_path;
};