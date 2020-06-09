#include "sdfu_types.h"

#include <bitsery/bitsery.h>
#include <iostream>

#include <bitsery/ext/std_variant.h>
#include <bitsery/ext/std_optional.h>

using namespace NRFDL::SDFU;

namespace NRFDL::SDFU
{
    template <typename S> void serialize(S & s, DfuResponseProtocol & o)
    {
        s.value1b(o.version);
    };

    template <typename S> void serialize(S & s, DfuResponseHardware & o)
    {
        s.value4b(o.part);
        s.value4b(o.variant);
        s.object(o.memory);
    };

    template <typename S> void serialize(S & s, DfuResponseHardwareMemory & o)
    {
        s.value4b(o.rom_size);
        s.value4b(o.ram_size);
        s.value4b(o.rom_page_size);
    };

    template <typename S> void serialize(S & s, DfuResponseFirmware & o)
    {
        s.value1b(o.type);
        s.value4b(o.version);
        s.value4b(o.addr);
        s.value4b(o.len);
    };

    template <typename S> void serialize(S & s, DfuResponseSelect & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
        s.value4b(o.max_size);
    };

    template <typename S> void serialize(S & s, DfuResponseCreate & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
    };

    template <typename S> void serialize(S & s, DfuResponseWrite & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
    };

    template <typename S> void serialize(S & s, DfuResponseCrc & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
    };

    template <typename S> void serialize(S & s, DfuResponsePing & o)
    {
        s.value1b(o.id);
    };

    template <typename S> void serialize(S & s, DfuResponseMtu & o)
    {
        s.value2b(o.size);
    };


    template <typename S> void serialize(S & s, DfuRequestFirmware & o)
    {
        s.value1b(o.image_number);
    };

    template <typename S> void serialize(S & s, DfuRequestSelect & o)
    {
        s.value4b(o.object_type);
    };

    template <typename S> void serialize(S & s, DfuRequestCreate & o)
    {
        s.value4b(o.object_type);
        s.value4b(o.object_size);
    };

    template <typename S> void serialize(S & s, DfuRequestWrite & o)
    {
        // TODO: fix removal of 1 byte additional length
        s.container1b(o.data, std::numeric_limits<uint16_t>::max());
        s.value2b(o.len);
    };

    template <typename S> void serialize(S & s, DfuRequestPing & o)
    {
        s.value1b(o.id);
    };

    template <typename S> void serialize(S & s, DfuRequestMtu & o)
    {
        s.value2b(o.size);
    };

    template <typename S> void serialize(S & s, DfuRequestPrn & o)
    {
        s.value4b(o.target);
    };


} // namespace NRFDL::SDFU


namespace bitsery
{
    namespace ext
    {
        class DfuRequestExtension
        {
          public:
            template <typename Ser, typename Fnc> void serialize(Ser & ser, const DfuRequest & obj, Fnc && fnc) const
            {
                // TODO: check that this function is receving the wrapper
                ser.value1b(obj.opcode);

                switch (obj.opcode)
                {
                    case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestFirmware>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestCreate>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_RECEIPT_NOTIF_SET:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestPrn>(*(obj.request)));
                        }
                        break;

                    case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestSelect>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_MTU_GET:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestMtu>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestWrite>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_PING:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestPing>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_EXECUTE:
                    case DfuOpcode::NRF_DFU_OP_HARDWARE_VERSION:
                    case DfuOpcode::NRF_DFU_OP_ABORT:
                    case DfuOpcode::NRF_DFU_OP_RESPONSE:
                    case DfuOpcode::NRF_DFU_OP_INVALID:
                    case DfuOpcode::NRF_DFU_OP_PROTOCOL_VERSION:
                    case DfuOpcode::NRF_DFU_OP_CRC_GET:
                        // opcodes without any extra arguments
                        break;
                }
            }

            // template <typename Des, typename T, typename Fnc> void deserialize(Des & des, T & obj, Fnc && fnc) const
            template <typename Des, typename Fnc> void deserialize(Des & des, DfuRequest & obj, Fnc && fnc) const
            {
                //res{};
                //fnc(des, res);
            }
        };

        class DfuResponseExtension
        {
          public:
            template <typename Ser, typename Fnc>
            void serialize(Ser & s, const DfuResponse & o, Fnc && ) const
            {
                s.value1b(o.opcode);
                s.value1b(o.result);

                // shouldn't you serialize if response is empty or not?
                // you can use StdOptional extension for this
                if (o.response.has_value())
                {
                    switch (o.opcode)
                    {
                        case DfuOpcode::NRF_DFU_OP_PROTOCOL_VERSION:
                            s.object(std::get<DfuResponseProtocol>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                            s.object(std::get<DfuResponseCreate>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_RECEIPT_NOTIF_SET:
                            break;
                        case DfuOpcode::NRF_DFU_OP_CRC_GET:
                            s.object(std::get<DfuResponseCrc>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_EXECUTE:
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                            s.object(std::get<DfuResponseSelect>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_MTU_GET:
                            s.object(std::get<DfuResponseSelect>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                            s.object(std::get<DfuResponseWrite>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_PING:
                            s.object(std::get<DfuResponsePing>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_HARDWARE_VERSION:
                            s.object(std::get<DfuResponseHardware>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                            s.object(std::get<DfuResponseFirmware>(*o.response));
                            break;
                        case DfuOpcode::NRF_DFU_OP_ABORT:
                        case DfuOpcode::NRF_DFU_OP_RESPONSE:
                        case DfuOpcode::NRF_DFU_OP_INVALID:
                            break;
                    }
                }
            }

            // template <typename Des, typename T, typename Fnc> void deserialize(Des & des, T & obj, Fnc && fnc) const
            template <typename Des, typename Fnc>
            void deserialize(Des & s, DfuResponse & o, Fnc && ) const
            {
                // same as in serialize function, except that this is called during deserialization
                s.value1b(o.opcode);
                s.value1b(o.result);
                // i'll assume that you will be using StdOptional for serialization, just to show how to use it :)
                // it reads first byte, and if it is ==1 then invoke lambda to deserialize its value
                s.ext(o.response, ext::StdOptional{}, [opcode= o.opcode](Des& s, DfuResponseType& v) {
                    // this will be different than serialization, because we need to set active variant element before deserializing
                    switch (opcode)
                    {
                        case DfuOpcode::NRF_DFU_OP_PROTOCOL_VERSION:
                            // set active variant element and deserialize to it
                            v = DfuResponseProtocol{};
                            s.object(std::get<DfuResponseProtocol>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                            v = DfuResponseCreate{};
                            s.object(std::get<DfuResponseCreate>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_RECEIPT_NOTIF_SET:
                            break;
                        case DfuOpcode::NRF_DFU_OP_CRC_GET:
                            v = DfuResponseCrc{};
                            s.object(std::get<DfuResponseCrc>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_EXECUTE:
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                            v = DfuResponseSelect{};
                            s.object(std::get<DfuResponseSelect>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_MTU_GET:
                            v = DfuResponseSelect{};
                            s.object(std::get<DfuResponseSelect>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                            v = DfuResponseWrite{};
                            s.object(std::get<DfuResponseWrite>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_PING:
                            v = DfuResponsePing{};
                            s.object(std::get<DfuResponsePing>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_HARDWARE_VERSION:
                            v = DfuResponseHardware{};
                            s.object(std::get<DfuResponseHardware>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                            v = DfuResponseFirmware{};
                            s.object(std::get<DfuResponseFirmware>(v));
                            break;
                        case DfuOpcode::NRF_DFU_OP_ABORT:
                        case DfuOpcode::NRF_DFU_OP_RESPONSE:
                        case DfuOpcode::NRF_DFU_OP_INVALID:
                            break;
                    }

                });
            }
        };
    }; // namespace ext

    namespace traits
    {
        template <typename T> struct ExtensionTraits<ext::DfuRequestExtension, T>
        {
            static_assert(std::is_same_v<T, DfuRequest>, "Only works with DfuRequest type");
            using TValue                                = T;
            static constexpr bool SupportValueOverload  = false;
            static constexpr bool SupportObjectOverload = true;
            static constexpr bool SupportLambdaOverload = false;
        };

        template <typename T> struct ExtensionTraits<ext::DfuResponseExtension, T>
        {
            static_assert(std::is_same_v<T, DfuResponse>, "Only works with DfuRequest type");
            using TValue                                = T;
            static constexpr bool SupportValueOverload  = false;
            static constexpr bool SupportObjectOverload = true;
            static constexpr bool SupportLambdaOverload = false;
        };

    } // namespace traits
} // namespace bitsery





namespace NRFDL::SDFU {

    template <typename S> void serialize(S & s, DfuResponse & o)
    {
        s.ext(o, bitsery::ext::DfuResponseExtension{});
    };

    template <typename S> void serialize(S & s, DfuResponseWrapper & o)
    {
        s.object(o.rsp);
    }

    template <typename S>
    void serialize(S& s, DfuRequest& o) {
        s.ext(o, bitsery::ext::DfuRequestExtension{});
    }

    template <typename S> void serialize(S & s, DfuRequestWrapper & o)
    {
        s.object(o.req);
    }
}
