# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/16 14:58:03 by ocmarout          #+#    #+#              #
#    Updated: 2021/12/10 14:49:04 by ocmarout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	main.c	\
				philo.c	\
				utils.c	\
				libft.c	\
				libft2.c

NAME		=	philo

OBJS		=	${SRCS:.c=.o}

OBJS_DIR	=	./.objs/

CC			=	clang

MKDIR		=	mkdir -p

CFLAGS		=	-Wall -Wextra -Werror -pthread ${INCLUDES}

INCLUDES	=	-I includes/

DEPF		= -MMD

DEP			= $(OBJS:.o=.d)

all:		${NAME}

bonus:		all

${NAME}:	$(addprefix ${OBJS_DIR}, ${OBJS})
		$(CC) $(DEPF) -o $@ $^ ${CFLAGS}

${OBJS_DIR}:
		${MKDIR} ${OBJS_DIR}

${OBJS_DIR}%.o:	${SRCS_DIR}%.c | ${OBJS_DIR}
		${CC} $(DEPF) ${CFLAGS} -c $< -o ${@}

clean:
		rm -rf ${OBJS_DIR}

fclean: clean
		rm -f ${NAME}

re:		fclean all

norme:
		norminette includes/
		norminette ${SRCS}

-include $(addprefix ${OBJS_DIR}, ${DEP})

.PHONY:	all clean fclean re bonus norme
