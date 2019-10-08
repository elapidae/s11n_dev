#!/bin/bash

#G=grep\ --invert-match\ \'#include\ \"impl/\'

O=form_as_one_file.cpp 


cat ../impl_s11n/str_view.h >$O

cat ../impl_s11n/void_type.h                |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/crc.h                      |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/name_of_type_from_pf.h     |grep --invert-match '#include "impl_s11n/' >>$O

cat ../impl_s11n/tuple_helper.h             |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/container_helper.h         |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/metaargs_helper.h          |grep --invert-match '#include "impl_s11n/' >>$O

cat ../impl_s11n/signature_1_name_of_type.h |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/signature_2_metatype.h     |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/signature_3_serial_tuple.h |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/signature.h                |grep --invert-match '#include "impl_s11n/' >>$O

cat ../impl_s11n/endian.h                   |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/reader.h                   |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/writer.h                   |grep --invert-match '#include "impl_s11n/' >>$O

cat ../impl_s11n/type_spec.h                |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/size.h                     |grep --invert-match '#include "impl_s11n/' >>$O

cat ../impl_s11n/encode.h                   |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/decode.h                   |grep --invert-match '#include "impl_s11n/' >>$O

cat ../impl_s11n/std_list.h                 |grep --invert-match '#include "impl_s11n/' >>$O
cat ../impl_s11n/std_string.h               |grep --invert-match '#include "impl_s11n/' >>$O


cat ../s11n.h                               |grep --invert-match '#include "impl_s11n/' >>$O


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

