#!/bin/bash

#G=grep\ --invert-match\ \'#include\ \"impl/\'

O=form_as_one_file.cpp 


cat ../impl/str_view.h                   >$O

cat ../impl/void_type.h                 |grep --invert-match '#include "impl/' >>$O
cat ../impl/crc.h                       |grep --invert-match '#include "impl/' >>$O
cat ../impl/name_of_type_from_pf.h      |grep --invert-match '#include "impl/' >>$O

cat ../impl/tuple_helper.h              |grep --invert-match '#include "impl/' >>$O
cat ../impl/container_helper.h          |grep --invert-match '#include "impl/' >>$O
cat ../impl/metaargs_helper.h           |grep --invert-match '#include "impl/' >>$O

cat ../impl/signature_1_name_of_type.h  |grep --invert-match '#include "impl/' >>$O
cat ../impl/signature_2_metatype.h      |grep --invert-match '#include "impl/' >>$O
cat ../impl/signature_3_description.h   |grep --invert-match '#include "impl/' >>$O
cat ../impl/signature_4_serial_tuple.h  |grep --invert-match '#include "impl/' >>$O
cat ../impl/signature.h                 |grep --invert-match '#include "impl/' >>$O

cat ../impl/endian.h                    |grep --invert-match '#include "impl/' >>$O
cat ../impl/reader.h                    |grep --invert-match '#include "impl/' >>$O
cat ../impl/writer.h                    |grep --invert-match '#include "impl/' >>$O

cat ../impl/type_spec.h                 |grep --invert-match '#include "impl/' >>$O
cat ../impl/size.h                      |grep --invert-match '#include "impl/' >>$O

cat ../impl/encode.h                    |grep --invert-match '#include "impl/' >>$O
cat ../impl/decode.h                    |grep --invert-match '#include "impl/' >>$O

cat ../impl/std_list.h                  |grep --invert-match '#include "impl/' >>$O
cat ../impl/std_string.h                |grep --invert-match '#include "impl/' >>$O


cat ../s11n.h                           |grep --invert-match '#include "impl/' >>$O


echo >>$O
echo >>$O
echo >>$O
echo "//==================================================" >>$O
echo "//==================================================" >>$O
echo "#include <iostream>" >>$O
echo >>$O
echo "int main() {" >>$O
echo "    std::cout << s11n::signature_crc<int32_t>() << ' '" >>$O
echo "              << std::hex << s11n::signature_crc<int32_t>() << std::endl;" >>$O
echo "}" >>$O
echo "//==================================================" >>$O
echo "//==================================================" >>$O

