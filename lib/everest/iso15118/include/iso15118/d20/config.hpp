// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 Pionix GmbH and Contributors to EVerest
#pragma once

#include <cstdint>
#include <ctime>
#include <map>
#include <optional>
#include <vector>

#include <iso15118/d20/der_functions.hpp>
#include <iso15118/d20/limits.hpp>
#include <iso15118/message/common_types.hpp>

// EvseSetupConfig / SessionConfig (the universal SECC-side session configuration consumed by the
// ISO 15118-20, ISO 15118-2 and DIN SPEC 70121 SECC engines) live in the protocol-neutral
// iso15118::session namespace (see include/iso15118/session/config.hpp). The structs below remain in
// d20 as they are expressed with the -20 RationalNumber datatype / IEC 15118-20 DER control types, and
// are referenced qualified (d20::...) from the session-side structs.
namespace iso15118::d20 {

struct ControlMobilityNeedsModes {
    message_20::datatypes::ControlMode control_mode;
    message_20::datatypes::MobilityNeedsMode mobility_mode;
};

struct AcSetupConfig {
    uint32_t voltage;
    std::vector<message_20::datatypes::AcConnector> connectors;
};

struct BptSetupConfig {
    message_20::datatypes::BptChannel bpt_channel;
    message_20::datatypes::GeneratorMode generator_mode;
    std::optional<message_20::datatypes::GridCodeIslandingDetectionMethod> grid_code_detection_method;
};

struct DerIecSetupConfig {
    std::map<iec::DERControlName, iec::DERControlFunction> supported_der_control_functions;
    iec::OperatingMode operating_mode;
    iec::GridConnectionMode grid_connection_mode;
};

struct DerSaeSetupConfig {
    explicit DerSaeSetupConfig(sae::DERControl der_control_, sae::RequiredDEROperatingMode op_mode,
                               sae::GridConnectionMode conn_mode) :
        der_control(std::move(der_control_)),
        required_der_operating_mode(op_mode),
        grid_connection_mode(conn_mode),
        der_control_update_time(static_cast<uint64_t>(std::time(nullptr))){};

    DerSaeSetupConfig() = delete;
    // TODO(SL): Check if copy or move constructor should also be removed?

    sae::DERControl der_control;
    sae::RequiredDEROperatingMode required_der_operating_mode;
    sae::GridConnectionMode grid_connection_mode;
    uint64_t der_control_update_time{0}; // SECC time
};

} // namespace iso15118::d20
