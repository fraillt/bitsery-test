#include "sdfu_codec.h"
#include "nrfdl_types.h"

#include "sdfu_codec_bitsery.h"

#include <bitsery/bitsery.h>

#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/string.h>
#include <bitsery/traits/vector.h>

namespace NRFDL::SDFU
{
    struct BitseryConfig
    {
        static constexpr bitsery::EndiannessType Endianness = bitsery::EndiannessType::LittleEndian;
        static constexpr bool CheckAdapterErrors            = true;
        static constexpr bool CheckDataErrors               = true;
    };

    Codec::Codec()
    {
        //_logger = spdlog::default_logger();
    }

    auto Codec::encode(const DfuRequestWrapper & request, data_t & packet) -> nrfdl_errorcode_t
    {
        using OutputAdapter = bitsery::OutputBufferAdapter<data_t, BitseryConfig>;
        auto writtenSize    = bitsery::quickSerialization<OutputAdapter>(packet, request);
        packet.resize(writtenSize);
        //_logger->debug("Encoded request into {} bytes.", writtenSize);
        return NRFDL_ERR_NONE;
    }

    auto Codec::decode(const data_t & packet, DfuResponseWrapper & response) -> nrfdl_errorcode_t
    {
        using InputAdapter = bitsery::InputBufferAdapter<data_t, BitseryConfig>;

        auto state = bitsery::quickDeserialization<InputAdapter>({packet.begin(), packet.size()}, response);
        if (!(state.first == bitsery::ReaderError::NoError && state.second))
        {
          //  _logger->error("Error parsing response");
            return NRFDL_ERR_PROTOCOL;
        }

        return NRFDL_ERR_NONE;
    }
} // namespace NRFDL::SDFU
