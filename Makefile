# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/16 14:58:03 by ocmarout          #+#    #+#              #
#    Updated: 2021/11/20 17:31:23 by ocmarout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	$(addprefix, philo.c)

NAME		=	philo

OBJS		=	${SRCS:.c=.o}

OBJS_DIR	=	./.objs/

CC			=	clang

MKDIR		=	mkdir -p

CFLAGS		=	-Wall -Wextra -Werror ${INCLUDES}

INCLUDES	=	-I includes/

all:		${NAME}

bonus:		all

${NAME}:	$(addprefix ${OBJS_DIR}, ${OBJS})
		$(CC) -o $@ $^ ${CFLAGS}

${OBJS_DIR}:
		${MKDIR} ${OBJS_DIR}

${OBJS_DIR}%.o:	${SRCS_DIR}%.c | ${OBJS_DIR}
		${CC} ${CFLAGS} -c $< -o ${@}

clean:
		make clean -C libft/
		rm -rf ${OBJS_DIR}

fclean: clean
		make fclean -C libft/
		rm -f ${NAME}

re:		fclean ${OBJS_DIR} all

norme:
		norminette ${SRCS}
