//
// Created by xjz on 2022/5/29.
//

#ifndef SDXX_LOG_H
#define SDXX_LOG_H

#define ERROR_LOG(file) fprintf(file,"[%s %s] %s: %s: %d\n", \
			__DATE__, __TIME__, __FILE__, __func__, __LINE__);

#endif //SDXX_LOG_H
