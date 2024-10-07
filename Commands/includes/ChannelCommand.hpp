#pragma once

#include "ICommand.hpp"

class	ChannelCommand : public ICommand
{
	public:
		virtual	~ChannelCommand();

		virtual void	execute() = 0;
		virtual bool	validate() = 0;
};
