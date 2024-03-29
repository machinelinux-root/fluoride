/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fields/custom_field_fixed_size.h"
#include "util.h"

const std::string CustomFieldFixedSize::kFieldType = "CustomField";

CustomFieldFixedSize::CustomFieldFixedSize(std::string name, std::string type_name, int size, ParseLocation loc)
    : ScalarField(name, size, loc), type_name_(type_name) {}

const std::string& CustomFieldFixedSize::GetFieldType() const {
  return CustomFieldFixedSize::kFieldType;
}

std::string CustomFieldFixedSize::GetDataType() const {
  return type_name_;
}

void CustomFieldFixedSize::GenExtractor(std::ostream& s, Size start_offset, Size end_offset) const {
  int field_size = GetSize().bits();

  if (!start_offset.empty()) {
    // Default to start if available.
    s << "auto it = begin_it + (" << start_offset << ") / 8;";
  } else if (!end_offset.empty()) {
    Size byte_offset = Size(field_size) + end_offset;
    s << "auto it = end_it - (" << byte_offset << ") / 8;";
  } else {
    ERROR(this) << "Ambiguous offset for field.";
  }

  s << GetDataType() << " value = it.extract<" << GetDataType() << ">();";
}

bool CustomFieldFixedSize::HasParameterValidator() const {
  return false;
}

void CustomFieldFixedSize::GenParameterValidator(std::ostream&) const {
  // Do nothing.
}

void CustomFieldFixedSize::GenValidator(std::ostream&) const {
  // Do nothing.
}
